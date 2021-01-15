/********************* (C) COPYRIGHT 2018 e-Design Co.,Ltd. ********************
  DS213_APP Process.h                                            Author : bure
*******************************************************************************/
#ifndef __Process_h
#define __Process_h

  #ifdef STM32F30X
    #include "stm32f30x.h"
  #elif STM32F10X_HD
    #include "stm32f10x.h"
  #endif

  #define CH_A       0
  #define CH_B       1
  #define CH_C       2
  #define CH_D       3
  #define FINISH     0x08

  #define C_AMPL     20
  #define D_AMPL     20

  #define ROLL_200MS    2  // TB >= 200ms/div
  #define ROLL_20MS     5  // TB >= 20ms/div

  typedef enum {
    RECOD1 = 2, RECOD2, RECOD3, RECOD4,
  } track3_mode;

  typedef enum {
    A_ADD_B = 2, A_SUB_B, C_AND_D, C_OR_D, INV_A, INV_B,
  } track_mode;

  extern u8  Recod[], Trigg, ScrnF, Full, Empty, Frame;//, Measured;
  extern s16 MeasurY[4][9];
  extern u16 MeasurX[4][7];

  extern u16 posLast;

  extern void DispSync(void);
  extern void CtrlUpdate(u8 Item);
  extern u8 isSynRoll(void);

#endif

/******************************** END OF FILE *********************************/
