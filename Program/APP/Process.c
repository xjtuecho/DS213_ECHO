/******************** (C) COPYRIGHT 2018 e-Design Co.,Ltd. *********************
  DS213_APP Process.c                                            Author : bure
*******************************************************************************/
#include "Process.h"
#include "DS213Bios.h"
#include "Sys.h"

void Y_Measure(void);
void RmsMeasure(u8 ChN);
void TimMeasure(u8 ChN);
void MeasureWaveForm(void);
u16  WaveFormExtract(u16 Xposn);

u8   Trigg = 0, ScrnF = 0, Full = 0, Psmpl = 0, Empty = 0, Frame = 0;
u8   Recod[4*X_SIZE+1]; // 波形记录缓冲区（从文件读入或截屏的同时写入）
s16  MeasurY[4][9];     // [CH_A~D][MAX/MIN/VPP/RMS/AVG/D_V/VTH/B_V/ACT]
u16  MeasurX[4][7];     // [CH_A~D][PTWH/PTWL/PCYC/CYCL/FPS]

u32  gDebug = 0;
u8   gDebugBuf[8] = {0};
u16  posLast = 0;

/*******************************************************************************
  统计显示窗中各波形的最大最小值，单位为 Y 坐标点数
*******************************************************************************/
void Y_Measure(void)
{
  u8* p = (u8*)Track;
  u32 SumA = 0, SumB = 0;

  MeasurY[CH_A][MAX] = 0, MeasurY[CH_A][MIN] = 255;
  MeasurY[CH_B][MAX] = 0, MeasurY[CH_B][MIN] = 255;

  MeasurY[CH_C][MAX] = Yn[CH_C]+C_AMPL, MeasurY[CH_C][MIN] = Yn[CH_C];
  MeasurY[CH_D][MAX] = Yn[CH_D]+D_AMPL, MeasurY[CH_D][MIN] = Yn[CH_D];

  for(u32 i = X_BASE; i < X_SIZE; i++){
    if(MeasurY[CH_A][MAX] < *p) MeasurY[CH_A][MAX] = *p;
    if(MeasurY[CH_A][MIN] > *p) MeasurY[CH_A][MIN] = *p;
    SumA += *p++;
    if(MeasurY[CH_B][MAX] < *p) MeasurY[CH_B][MAX] = *p;
    if(MeasurY[CH_B][MIN] > *p) MeasurY[CH_B][MIN] = *p;
    SumB += *p++;
    p += 2;
  }
  MeasurY[CH_A][ACT]  = (MeasurY[CH_A][MAX] < Y_SIZE) ? 1 : 0;
  MeasurY[CH_A][ACT] &= (MeasurY[CH_A][MIN] > Y_BASE) ? 1 : 0;

  if(MeasurY[CH_A][ACT]){
    MeasurY[CH_A][MAX] = MeasurY[CH_A][MAX]-Yn[CH_A];
    MeasurY[CH_A][MIN] = MeasurY[CH_A][MIN]-Yn[CH_A];
  }
  MeasurY[CH_A][VPP] = MeasurY[CH_A][MAX]-MeasurY[CH_A][MIN];
  MeasurY[CH_A][AVG] = SumA/X_SIZE-Yn[CH_A];

  MeasurY[CH_B][ACT]  = (MeasurY[CH_B][MAX] < Y_SIZE) ? 1 : 0;
  MeasurY[CH_B][ACT] &= (MeasurY[CH_B][MIN] > Y_BASE) ? 1 : 0;

  if(MeasurY[CH_B][ACT]){
    MeasurY[CH_B][MAX] = MeasurY[CH_B][MAX]-Yn[CH_B];
    MeasurY[CH_B][MIN] = MeasurY[CH_B][MIN]-Yn[CH_B];
  }
  MeasurY[CH_B][VPP] = MeasurY[CH_B][MAX]-MeasurY[CH_B][MIN];
  MeasurY[CH_B][AVG] = SumB/X_SIZE-Yn[CH_B];

  MeasurY[CH_A][VTH] = Vt[CH_A]-Yn[CH_A];
  MeasurY[CH_B][VTH] = Vt[CH_B]-Yn[CH_B];

  MeasurY[0][D_V]    = Menu[V_1].Val-Menu[V_2].Val;
  MeasurY[0][B_V]    = Vsum/8;
}

/*******************************************************************************
  测量当前显示窗中指定 ChN 通道波形的有效值，单位为 Y 坐标点数
*******************************************************************************/
void RmsMeasure(u8 ChN)
{
  s32 Mid = (MeasurY[ChN][MAX]+MeasurY[ChN][MIN])/2;
  u8* ptr = &Track[ChN];
  u32 Curr = 0, Last = 0, BgnF = 0, Ssum = 0, Savg = 0, Xcnt = 0;
  for(u32 x = X_BASE; x < X_SIZE; x++){
    s32 Yin = *ptr-Yn[ChN];
    Curr = (Yin > Mid) ? 1 : 0;
    if(x > X_BASE){                          // 忽略第 1 项数据
      if(BgnF != 0) Ssum += Yin*Yin, Xcnt++; // 周期开始后累加平方值
      if(Last != Curr){                      // 有跳变产生
        u32 Flag = Last*2+Curr;
        if(BgnF == Flag)   Savg = Ssum/Xcnt; // 第 n 周期结束计算平均值
        else if(BgnF == 0) BgnF = Flag;      // 第 0 周期开始置起始标志
      }
    }
    Last = Curr, ptr += 4;
  }
  MeasurY[ChN][RMS] = Sqrt32(Savg);
}
/*******************************************************************************
  测量当前显示窗中指定通道波形的时间参数，单位为 X 坐标点数
*******************************************************************************/
void TimMeasure(u8 ChN)
{
  u32 Mid = (MeasurY[ChN][MAX]+MeasurY[ChN][MIN])/2+Yn[ChN];
  u32 Curr = 0, Last = 0, BgnF = 0, Xcnt = 0, SumH = 0, SumL = 0;
  u32 TwsH = 0, TwsL = 0, TwsN = 0;

  u8* ptr = &Track[ChN];

  for(u32 x = X_BASE; x < X_SIZE; x++){
    if(*ptr > Mid+5) Curr = 1;
    if(*ptr < Mid-5) Curr = 0;
    if(x > X_BASE){                                   // 忽略第 1 项数据
      Xcnt++;                                         // 脉宽计数+1
      if(Last != Curr){                               // 有跳变产生
        u32 Flag = Last*2+Curr;
        if(BgnF == 0) BgnF = Flag;                    // 第 0 周期起始标志
        else {
          if(Curr == 0) SumH += Xcnt;                 // 累加正脉宽
          else          SumL += Xcnt;                 // 累加负脉宽
          if(BgnF == Flag){                           // 第 n 整数周期结束
            TwsN++;                                   // 周期数+1
            TwsH = SumH, TwsL = SumL;                 // 保存高低脉宽计数
          }
        }
        Xcnt = 0;                                     // 脉宽计数清零
      }
    }
    Last = Curr, ptr += 4;
  }
  u32 Mask = (TwsN != 0 && TwsN < 30)? 100 : 0;       // 为保证精度预乘 100
  MeasurX[ChN][PTWH] = Mask*TwsH/TwsN;
  MeasurX[ChN][PTWL] = Mask*TwsL/TwsN;
  MeasurX[ChN][CYCL] = Mask*(TwsH+TwsL)/TwsN;
  MeasurX[ChN][FQRN] = Mask*(TwsH+TwsL)/TwsN;
  MeasurX[ChN][DUTY] = Mask*TwsH/(TwsH+TwsL);
  MeasurX[0][D_T]    = 100*abs(Menu[T_2].Val-Menu[T_1].Val);
  MeasurX[0][FPS]    = Fps;
}
void MeasureWaveForm(void)
{
  Y_Measure();                                        // 垂直方向值的测量
  if(Menu[VM1].Val == RMS) RmsMeasure(Menu[VM1].Src); // 垂直方向的有效值
  if(Menu[VM2].Val == RMS) RmsMeasure(Menu[VM2].Src);
  if(Menu[TM1].Val != D_T) TimMeasure(Menu[TM1].Src); // 水平方向值的测量
  if(Menu[TM2].Val != D_T) TimMeasure(Menu[TM2].Src);
}

void DispDebugStr(void)
{
  u16To5DecStr(gDebugBuf, gDebug);
  RowPosi(Menu[TM2].X0, Menu[TM2].Y0);
  AddStr(TXT2C, CHAR, gDebugBuf);
}

// 波形滚动处理
void WaveFormRoll(u16 pos)
{
  u32 *src = (u32*)TrackRoll;
  u32 *dst = (u32*)Track;
  s32 i = 0, newNum = 0;

  if(pos >= posLast) {
    newNum = pos - posLast;
    for(i=0; i<X_SIZE; i++) {
        dst[i] = (i < X_SIZE-newNum) ? dst[i+newNum] : src[posLast + i - (X_SIZE - newNum)];
    }
  }
  posLast = (pos >= X_SIZE) ? X_BASE : pos;
}

/*******************************************************************************
  显示同步处理
*******************************************************************************/
void DispSync(void)
{
  u32 Status = FPGA_CtrlRW(CH_A, SMPL_ST) | FPGA_CtrlRW(CH_B, SMPL_ST);
  Trigg = (Status & TRIGG) ? 1 : 0;
  ScrnF = (Status & SCRNF) ? 1 : 0;
  Psmpl = (Status & PSMPL) ? 1 : 0;
  Full  = (Status & FULL ) ? 1 : 0;
  Empty = (Status & EMPTY) ? 1 : 0;

  u32 SyncM = Menu[SYN].Val;
  u32 Tbase = Menu[TIM].Val;
  u32 Xposn = Menu[XNP].Val;
  u32 ViewP = 30*Xposn*BASE_KP1[Tbase]/1024;         // 插值补偿后窗口位置
  u32 Npts = 0;
  switch(SyncM){
    case AUTO:
      // 时基 >= 200ms/div 使用滚屏模式
      if(Tbase <= ROLL_200MS){
        if(SMPL == Menu[RUN].Val){
          Npts = WaveFormExtract(0);                 // 从头开始提取
          WaveFormRoll(Npts);
        }
      } else {
        if(Trigg || (!Trigg && ScrnF && SMPL == Menu[RUN].Val)){
          Npts = WaveFormExtract(ViewP);             // 从指定窗口位置开始提取
        }
      }
      DisplayWaveForm();                             // 显示提取的波形
      MeasureWaveForm();
      if(X_SIZE == Npts){
        SmplStart();                                 // 重新采样
      }
      break;
    case NORM:
      if(Trigg){
        Npts = WaveFormExtract(ViewP);               // 从指定窗口位置开始提取
      }
      DisplayWaveForm();                             // 显示提取的波形
      MeasureWaveForm();
      if(X_SIZE == Npts){
        SmplStart();                                 // 重新采样
      }
      break;
    case SNGL:
      if(Trigg){
        Npts = WaveFormExtract(ViewP);               // 从指定窗口位置开始提取
      } else {
        TrackClr();                                  // 清除波形轨迹
      }
      if(Full){
        Menu[RUN].Val  = STOP,                       // 满屏后暂停
        Menu[RUN].Flg |= UPDT;                       // 刷新显示暂停标志
      }
      DisplayWaveForm();                             // 显示提取的波形
      MeasureWaveForm();
      break;
    case ROLL:
      if(SMPL == Menu[RUN].Val){
        Npts = WaveFormExtract(0);                   // 从头开始提取
        WaveFormRoll(Npts);
      }
      DisplayWaveForm();                             // 显示提取的波形
      MeasureWaveForm();
      if(X_SIZE == Npts){
        SmplStart();                                 // 重新采样
      }
      break;
    default:
      break;
  }
}
/*******************************************************************************
  显示波形数据限幅
*******************************************************************************/
u16 DataLimit(s16 Data)
{
  return ((Data >= Y_SIZE)? Y_SIZE : (Data <= Y_BASE) ? Y_BASE : Data);
}

// 判断是否滚屏模式
u8 isSynRoll(void)
{
  return (ROLL == Menu[SYN].Val || (AUTO == Menu[SYN].Val && Menu[TIM].Val <= ROLL_200MS)) ? 1 : 0;
}

/*******************************************************************************
  显示波形提取与插值
*******************************************************************************/
u16 WaveFormExtract(u16 Xposn)
{
  FPGA_DataWr(A_C_CH, SMPL_RPTR, Xposn);              // 当前窗口位置读指针
  FPGA_DataWr(B_D_CH, SMPL_RPTR, Xposn);

  s32 GainA  = pGain[CH_A*10+Menu[RNA].Val];          // A 通道当前档位增益系数
  s32 GainB  = pGain[CH_B*10+Menu[RNB].Val];          // B 通道当前档位增益系数
  u32 Posn1  = Yn[TRCK1], Posn2 = Yn[TRCK2];          // 波形轨迹 1, 2 垂直零位
  u32 Posn3  = Yn[TRCK3], Posn4 = Yn[TRCK4];          // 波形轨迹 3, 4 垂直零位

  s32 i = BASE_KP1[Menu[TIM].Val];                    // 当前档位插值系数
  u32 n = Menu[T3S].Val;                              // n = 2~7 对应于各类运算
  u32 m = Menu[T4S].Val;                              // m = 2~5 对应于记录 0~3
  u8* q = (u8*)&Recod[m-2];
  u32 r = Recod[4*359+m-2];
  u32 x = 0, k, LastA, LastB, LastC, LastD;
  u8* p = (u8*)Track;

  if(isSynRoll()){
    p = TrackRoll;
  }

  while(1){
    u32 SmplAC = FPGA_SmplRd(A_C_CH);
    u32 SmplBD = FPGA_SmplRd(B_D_CH);
    u16 Status = SmplAC | SmplBD;
    Trigg = (Status & TRIGG) ? 1 : 0;
    ScrnF = (Status & SCRNF) ? 1 : 0;
    Full  = (Status & FULL ) ? 1 : 0;
    Psmpl = (Status & PSMPL) ? 1 : 0;
    Empty = (Status & EMPTY) ? 1 : 0;
    if(Empty) return x;                                // 缓冲区读空即退出

    s32 AmplA = CALIBRATE((SmplAC >> 8)-Posn1, GainA); // A 通道振幅提取校正
    u32 CurrA = DataLimit(AmplA+Posn1);                // A 通道显示数据限幅
    s32 AmplB = CALIBRATE((SmplBD >> 8)-Posn2, GainB); // B 通道振幅提取校正
    u32 CurrB = DataLimit(AmplB+Posn2);                // B 通道显示数据限幅

    s32 AmplD = (m > 1) ? *q-r : (SmplBD & 1)*D_AMPL;  // D 通道提取
    u32 CurrD = DataLimit(AmplD+Posn4);                // D 通道显示数据限幅

    s32 AmplC = (SmplAC & 1)*C_AMPL;                   // C 通道振幅提取
    switch (n){                                        // C 通道运算
      case A_ADD_B: AmplC = AmplA + AmplB; break;
      case A_SUB_B: AmplC = AmplA - AmplB; break;
      case C_AND_D: AmplC = AmplC & AmplD; break;
      case C_OR_D:  AmplC = AmplC | AmplD; break;
      case INV_A:   AmplC = -AmplA; break;
      case INV_B:   AmplC = -AmplB; break;
    }
    u32 CurrC = DataLimit(AmplC+Posn3);                // C 通道显示数据限幅

    if(x == X_BASE){                                   // 数值初始化
      *p++ = CurrA, *p++ = CurrB, *p++ = CurrC, *p++ = CurrD;
      k = 0, x++;
    } else {
    #if 1
      s32 DiffA = CurrA-LastA, DiffB = CurrB-LastB;
      s32 DiffC = CurrC-LastC, DiffD = CurrD-LastD;
      while(k <= 1024){                                // 插值运算
        if(m > 1) q += 4;
        u32 InstA = LastA+DiffA*k/1024, InstB = LastB+DiffB*k/1024;
        *p++ = InstA, *p++ = InstB;
        u32 InstC = LastC+DiffC*k/1024, InstD = LastD+DiffD*k/1024;
        *p++ = InstC, *p++ = InstD;
        k += i;
        if(++x == X_SIZE) return x;
      }
      k -= 1024;
    #else
      *p++ = CurrA, *p++ = CurrB, *p++ = CurrC, *p++ = CurrD;
      if(++x == X_SIZE) return x;
    #endif
    }
    LastA = CurrA, LastB = CurrB, LastC = CurrC, LastD = CurrD;
  }
}
/*******************************************************************************
  Update_Range:
*******************************************************************************/
void CtrlUpdate(u8 Item)
{
  u32 Value = Menu[Item].Val;
  u32 Tw;

  switch(Item){
    case RUN:
      if(Menu[RUN].Val == SMPL){
        SmplStart();                             // 重新开始采样
        TrackClr();                              // 清除显示波形轨迹
      }
      break;
    case CPA:
      *Hw.pOut_A_Coupl = Value & 1, YnHide[TRCK1] = !Value; // If "--" then Hide Trakc#1
      break;
    case CPB:
      *Hw.pOut_B_Coupl = Value & 1, YnHide[TRCK2] = !Value; // If "--" then Hide Trakc#2
      break;
    case RNA:
      SetRangeA(Value), SetOffsetA(Value, Yn[CH_A]);
      break;
    case RNB:
      SetRangeB(Value), SetOffsetB(Value, Yn[CH_B]);
      break;
    case T3S:
      YnHide[TRCK3] = !Value;                    // If "--" then Hide Trakc#3
      break;
    case T4S:
      YnHide[TRCK4] = !Value;                    // If "--" then Hide Trakc#4
      break;
    case YNP:
    case CAL:
      SetOffsetA(Menu[RNA].Val, Yn[CH_A]);
      SetOffsetB(Menu[RNB].Val, Yn[CH_B]);
      break;
    case T_0:
      SetPreSmplDpth(Value*30);                  // 预采样深度
      break;
    case V_T:
      FPGA_ByteWr(A_C_CH, TRIG_VOLT, Vt[CH_A]);
      FPGA_ByteWr(B_D_CH, TRIG_VOLT, Vt[CH_B]);
      break;
    case T_1:
    case T_2:
      if(Menu[T_1].Val > Menu[T_2].Val)
        Tw = Menu[T_1].Val-Menu[T_2].Val;
      else
        Tw = Menu[T_2].Val-Menu[T_1].Val;
      FPGA_DataWr(A_C_CH, TRIG_WDTH, Tw);
      FPGA_DataWr(B_D_CH, TRIG_WDTH, Tw);
      break;
    case OUT:
    case FRQ:
      if(Menu[OUT].Val == DGTL)
        SetDgtlFrq(Menu[FRQ].Val);
      else
        SetAnlgFrq(Menu[FRQ].Val);
      break;
    case TIM:
      SetBase(Value);
      break;
    case TRG:
      SetTriggTyp(Menu[TRG].Val, Menu[V_T].Src);
      break;
    default:
      break;
  }
  SetTriggTyp(isSynRoll() ? TRIG_TYPE_ANY : Menu[TRG].Val, Menu[V_T].Src);
}
/******************************** END OF FILE *********************************/
