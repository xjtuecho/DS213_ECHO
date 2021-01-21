/********************* (C) COPYRIGHT 2018 e-Design Co.,Ltd. ********************
  DS213_APP main.c                                               Author : bure
********************************************************************************
                               Edit notes
  ----------------------------------------------------------------------------
  18.07.25 APP V1.01: Initialization adds "Turn on V- and ADC" operation
                      AutoCalib operation resumed during initialization
  18.07.27 APP V1.02: Modify the logic of the 10V gear electronic switch
                      Fixed no sound of power-on button
  18.07.28 APP V1.03: Fix battery voltage reading
                      Save the file after automatic calibration when there is no configuration file at startup
                      The configuration file does not retain BAK (only one file can be kept valid, and the accumulation of cluster numbers may cause problems)
                      Perfect LED breathing effect during standby
                      Fix that when "XP" moves to the end and disconnects the input signal, the waveform cannot be cleared
                      singleMode lock, sampling status bitFINISH==FULL？
                      Increase volume and backlight parameter save
                      Improve the operation function of CH_D
                      Improve the analog waveform output of the output port
  18.08.08 APP V1.04: Improve FAT12 file format and file saving type
  18.08.10 APP V1.05: When increasing CAL, press Shift when the encoder accumulates (but there is no maximum and minimum limit)
                      Fix T0 cursor out of bounds and trigger position
                      Adjust SCAN, cancel NONE synchronization mode, SCAN and other synchronization methods are improved after correction? ? ?
                      Increase DUT, TH, TL
                      <Vt trigger mode is unstable, others are ok
  18.08.14 APP V1.06: Save the file starting from number 0
                      Improve the csv file save format
                      Improve saveDat, loadDat and CH3 recall record waveform
                      Automatically accumulate number after file is saved
  18.08.15 APP V1.07: Save file completion prompt
                      Remove TWTH
  18.08.15 APP V1.08: Add automatic calibration at startup
                      Fix Buf save format
                      Adjust sampling waiting time
  18.08.17 APP V1.09: Correct the trigger position of T0, (the time base is below 10us due to the difference algorithm, there is an offset between T0 and the trigger position, unresolved???)

  18.08.27 APP V1.10: Fix Load DAT problem
                      Increase the trigger voltage value display
                      Increase standby time menu selection
  18.08.29 APP V1.11: Fixed breathing light when not in standby
                      Increase battery voltage display

  18.09.03 APP V1.12: Revise the function of keeping parameters in standby time
                      Add K4 button sound feedback
                      When the increase is not unlocked, long press K4+K3 to shut down (maybe the hardware does not support it)

  18.09.27 APP V1.13: Repair low battery and restart repeatedly, the battery will automatically shut down when the battery is less than 3v
                      0.5us，0.2us，0.1us The time base is modified to500ns，200ns，100ns
                      Save parameter settings display save status
                      Change 1234 displayed on the screen channel to ABCD
           APP V1.14: Repair low battery restarts repeatedly, automatic shutdown when the battery is less than 3.2v

  18.11.09 APP V1.15: If there is no power-on parameter configuration file, only call the power-on calibration and save the power-on parameters
                      Eliminate the screen display residue when saving parameters, and extend the information display stay time
                      Analog output waveform sampling points, 50 points at 20KHz gear, 100 points at other gears

  18.12.22 APP V1.20: Rewrite the program with DFU3.73 and FPGA STMU_213_016.bin
                      Modified various problems in APP V1.xx

  19.02.25 APP V1.21: Modified the problem that the displayed waveform cannot be maintained in NORM mode
                      Modified the problem of automatically exiting the STOP state after other operations after pressing the K4 key

  19.03.08 APP V1.22: Fixed the problem that some machines with large offset errors cannot be automatically corrected
                      Modified the waveform display mode in various synchronization modes

  20.03.06 APP V1.23: Modified the frequency measurement error when changing the time base after a pause, and changed it to lock the time base after a pause
                      Modified the frequency measurement error problem when the time base is too low, and changed it to stop the measurement when the time base is too low
                      Cooperate with STMU_213_018.bin to modify the display error when single trigger

  20.03.12 APP V1.24: The reference voltage is different due to the replacement of MXT2088, the voltage correction coefficient table K1 is modified
                      Re-calibrate the HWD9288 voltage coefficient table and return the ADC model number according to DFU
                      Character string recognition automatically selects the corresponding correction coefficient table

  20.03.12 APP V1.25: According to the ADC string in the DFU, the corresponding voltage coefficient table is automatically called, and the
                      The problem of using U disk to set parameters.

  20.06.24 APP v1.26: Increase the gear position when the channel A and channel B probe attenuation 10 times
                      Fixed the problem that the SaveBuf() and SaveCSV() function calls were stuck.

  20.06.24 APP v1.27: Modified the ADC calibration parameters of MXT2088, and corrected the display error of the calculated value when the attenuation is 10 times.
*******************************************************************************/
#include "Sys.h"
#include "DS213Bios.h"


HwDrvDef  Hw;
u8  APP_VERSION[12] = { "v20.11.26" };
u8  PwrDownSt = 0;

/*******************************************************************************

*******************************************************************************/
int main(void)
{
  pFunc1 __BiosInit = (pFunc1)0x08000101; // Point to DFU BiosInit
  __BiosInit((u32)&Hw);                   // Load hardware device driver parameters

  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x8000);
  while(SysTick_Config(SystemCoreClock/1000)){};

/****** Only used when APP alone DEBUG debugging and running, this part will not be compiled and run when called by DFU ****

  Hw.pDevInit(PWRCTRL);
  Hw.pDevInit(KEYnDEV);
  Hw.pDevInit(DISPDEV);
  Hw.pDevInit(BUZZDEV);
  Hw.pClrScrn(BLK);
  u16 x0 = Hw.LogoX0, y0 = Hw.LogoY0;
  Hw.pDispLogo(x0, y0);
  *Hw.pPwm_Bkl = 50;
  *Hw.pBuz_Vol = 50;
  Beep_mS(500);
  Hw.pDevInit(FPGADEV);
  Hw.pDevInit(FPGACFG);
  */
/******************************************************************************/

  DispStr(12*8, 90, WHT, CHAR, "Oscilloscope APP");
  DispStr(29*8, 90, WHT, CHAR,  APP_VERSION );

  Hw.pDevInit(USBDISK);
  Hw.pDevInit(USB_DEV);
  USB_Init();
  AppDiskInit();
  InitFatParam();

  *Hw.pOut_nPD = 1;                                // Turn on V- and ADC
  Hw.pDevInit(SGNL_IN);                            // Signal input channel initialization
  Hw.pDevInit(SGNLOUT);                            // Signal output channel initialization
  Hw.pDevInit(SO_ANLG);                            // Signal output is analog signal
  Hw.pDevInit(SO_DGTL);                            // The signal output is a digital signal
  FPGA_CtrlRW(A_C_CH, CHIP_RST);                   // FPGA #0 module reset
  FPGA_CtrlRW(B_D_CH, CHIP_RST);                   // FPGA #1 module reset
  SmplStart();                                     // FPGA starts a new round of sampling
  TrackClr();                                      // Clear display waveform trace

  pGain = (strncmp((char*)Hw.pAdcTypStr, "AD9288",6)  == 0) ? &Gain0[0][0] :
          (strncmp((char*)Hw.pAdcTypStr, "HW9288",6)  == 0) ? &Gain1[0][0] :
          (strncmp((char*)Hw.pAdcTypStr, "MXT2088",7) == 0) ? &Gain2[0][0] :
          &Gain0[0][0];

  uc8 STR1[] = "Parameter record not found";
  uc8 STR2[] = "Reload parameter from disk";
  uc8* STR   = (LoadParam()) ? STR1 : STR2;        // Read the default boot parameters
  DispStr(12*8, 30, GRN, CHAR, (u8*)STR);          // Display reading result information
  *Hw.pPwm_Bkl = Pop[SBKL].Val;                    // Set backlight brightness
  *Hw.pBuz_Vol = Pop[SVOL].Val;                    // Set the buzzer volume
  if(STR == STR1){                                 // No preset boot parameter configuration file
    RowPosi(12*8, 50);
    AddStr(TXT2C, CHAR, (u8*)"Correction");
    Correction();                                  // Zero point correction for each gear
  } else Delay_mS(2000);

  //****************************** System main loop program ******************************
  SmplStart();                                                // Start sampling
  TrackClr();
  KeyAct = 0;
  while (1){
    UpdtMainMenu();                                           // Update the main interface menu
    DispSync();                                               // Display synchronization
    if(PopUpdt){
      UpdtPopMenu();                                          // Update popup menu
      PopUpdt = 0;
    }
    //============================== Key processing cycle ==============================
    u16 Act = KeyAct & (ENCDBIT | K1_ACTn | K2_ACTn | K3_ACTn);
    KeyAct = 0;
    if(Act){                                                  // There are buttons or encoder actions
      u32 Track = Menu[Item].Src;
      u32 Range = Menu[(Track == CH_A)? RNA : RNB].Val;
      u16 *pKn  = &Menu[Item].Val;
      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ K1 Push ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      if(Act & K1_ACTn){
        if(~nKeySt & K4){                                     // Press K1 while holding K4
          u32 r = Snapshot();                                 // Save the current screenshot
          RowPosi(Menu[TM2].X0, Menu[TM2].Y0);
          if(r == OK) AddStr(TXT2C, CHAR, " Saveing OK  ");
          else        AddStr(WARNG, CHAR, " Saveing Err ");
          Delay_mS(2000);
          Menu[TM2].Flg |= UPDT;                              // Set the update flag
        } else {                                              // Press K1 alone
          Menu[RUN].Val = (Menu[RUN].Val)? STOP : SMPL;       // Toggle pause/run status
          Menu[RUN].Flg |= UPDT;                              // Set the update flag
        }
      }
      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ K2 Push ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      if(Act & K2_ACTn){
        if(~nKeySt & K4){                                     // Press K2 while holding K4
          u32 r = SaveParam();                                // Save the current setting parameters
          RowPosi(Menu[TM2].X0, Menu[TM2].Y0);
          if(r == OK) AddStr(TXT2C, CHAR, " File Saved  ");
          else        AddStr(WARNG, CHAR, " Save Error  ");
          Delay_mS(2000);
          Menu[TM2].Flg |= UPDT;                              // Restore TM2 display items
        } else {                                              // Press K2 alone
          PopHide = (PopHide) ? 0 : 1;                        // Open/close pop-up window
          if(!PopHide) UpdtPopMenu();                         // Update popup menu
        }
      }
      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ K3 Push ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      if(Act & K3_ACTn){
        if(~nKeySt & K4){                                     // Press K3 while holding K4
          if(Hw.LicenceOk == 0) *Hw.pOut_PEn = 0;             // Test shutdown before unlocking
          else {
            RowPosi(12*8, 120);
            AddStr(TXT2C, CHAR, (u8*)"Correction");
            Correction();                                     // Zero point correction for each gear
            RowPosi(23*8, 120);
            AddStr(TXT2C, CHAR, "Completed    ");
            Delay_mS(2000);
          }
        } else {                                              // Press K3 alone
          if(PopHide){                                        // Operation under the main menu
            if(Item == V_T || Item == TRG){
              Menu[Item].Flg |= UPDT, Menu[Item].Src += 1;
              Menu[Item].Src &= 3;                            // select CH A~D
              Menu[V_T].Src = Menu[Item].Src;
              Menu[V_T].Flg |= UPDT;                          // Three data synchronization
              Menu[TRG].Src = Menu[Item].Src;
              Menu[TRG].Flg |= UPDT;
              Menu[T_0].Src = Menu[Item].Src;
              Menu[T_0].Flg |= UPDT;
            } else if(Item == YNP || Item == TM1 || Item == TM2){
              Menu[Item].Src += 1;
              Menu[Item].Src &= 3;                            //select CH A~D
              Menu[Item].Flg |= UPDT;
            } else if(Item == VM1 || Item == VM2 || Item == CAL){
              Menu[Item].Src += 1;
              Menu[Item].Src &= 1;                            // select CH A/B
              Menu[Item].Flg |= UPDT;
            } else if(Item >= V_1 && Item <= T_2) Menu[Item].Flg ^= INVR;
          } else PopProcess();                                // Operation in the pop-up window
        }
      }
      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Indx Encoder ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      if(Act & ENCD_1n || Act & ENCD_1p){
        u32 Inc   = (~nKeySt & K4) ? 10 : 1;                  // Increment when K4 is pressed *10
        if(PopHide){                                          // Operations under the main menu
          Menu[Item].Flg |= UPDT;
          if     (Item == V_T) pKn = &Vt[Track];
          else if(Item == YNP) pKn = &Yn[Track];
          else if(Item == CAL) pKn = &Diff[Track][Range];
          if((Item == VM1 || Item == VM2)&& *pKn == RMS){     // When leaving the effective value measurement
            Menu[T_1].Flg &= ~INVR, Menu[T_2].Flg &= ~INVR;   // Blanking T1, T2 cursors
          }
          u32 Undo1 = (Track >= CH_C && Item == V_T)? 1 : 0;  // Digital channel does not adjust V_T
          u32 Limit = (Item <= RNB || Item == TIM)  ? 1 : 0;
          u32 Undo2 = (Menu[RUN].Val == STOP && Limit);       // No range adjustment during pause
          if(!Undo1 && !Undo2){
            if(Act & ENCD_1n) *pKn =IndxInc(*pKn, Inc, Item); // Indx+
            else              *pKn =IndxDec(*pKn, Inc, Item); // Indx-
          }
          if(Menu[SYN].Val == NORM && Limit) TrackClr();      // NORM Clear screen patch

          if(Menu[OUT].Val < 3 && Menu[FRQ].Val > 10){        // OUT patch
            Menu[FRQ].Val  = 10;                              // limit <=20KHz
            Menu[FRQ].Flg |= UPDT;
          }
          if(Menu[SYN].Val == ROLL){                          // ROLL limit >=20mS
            if(Menu[TIM].Val > ROLL_20MS) Menu[TIM].Val = ROLL_20MS;
            Menu[TIM].Flg |= UPDT;
            Menu[XNP].Val = 0, Menu[XNP].Flg |= UPDT;         // Limit window position
          }
          if(Menu[SYN].Val <= NORM){                          // AUTO, NORM patch
            if(Menu[RUN].Val == STOP && !Trigg){
              Menu[XNP].Val = 0;                              // Limit window position
              Menu[XNP].Flg |= UPDT;
            }
          }
          //Judge whether it is necessary to switch the string of voltage selection level
          updata_RNAB_p();
        } else {                                              // Operations under the pop-up window
          PopUpdt = 1;
          if(Act & ENCD_1n) PopValInc(Line, Inc);             // Line+
          else              PopValDec(Line, Inc);             // Line-
        }
      }
      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Item+ Encoder ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      if(Act & ENCD_2p){
        Menu[Item].Flg |= FLSH;                               // Set flashing flag in front item
        if(PopHide){                                          // Operations under the main menu
          Item = (Item >= VM1) ? 0 : Item+1;                  // Current item +1
        } else {                                              // Operations under the pop-up window
          Line = (Line == SPDT) ? 0 : Line+1;                 // Current row +1
          PopUpdt = 1;
        }
      }
      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Item- Encoder ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      if(Act & ENCD_2n){
        Menu[Item].Flg |= FLSH;                               // Set flashing flag in front item
        if(PopHide){                                          // Operations under the main menu
          Item = (Item > 0) ? Item-1 : VM1;                   // Current item-1
        } else {                                              // Operations under the pop-up window
          Line = (Line > 0) ? Line-1 : SPDT;                  // Current row-1
          PopUpdt = 1;
        }
      }
      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      Beep_mS(20);
      if(Pop[SPDT].Val != 0) PD_Cnt = Pop[SPDT].Val*60;       // Reset sleep timer
    }
    //==============================f the channel has signal input, reset the count value===================
    if(*Hw.pSt_Vin == 1){
      PD_Cnt = Pop[SPDT].Val*60;                              // Reset sleep timer

    }

    //==============================Determine whether to enter power saving mode=============================
    if(PD_Cnt == 0 && PwrDownEn && *Hw.pSt_Vin == 0){         // Enter power saving state
      Hw.pPowerDown(ENABLE);                                  // Low power consumption mode
      *Hw.pPwm_Bkl = 0;                                       // Turn off the backlight
      *Hw.pOut_Clr = 1;                                       // FPGA Power saving status
      PwrDownSt = 1;
    } else if(PwrDownSt == 1){                                // Restore operation
      Hw.pPowerDown(DISABLE);                                 // Normal power supply mode
      *Hw.pPwm_Bkl = Pop[SBKL].Val;                           // Restore backlight
      *Hw.pOut_Clr = 0;                                       // FPGA Operating status
      *Hw.pPwm_LED = LED_PWM_MAX;                             // Turn off the breathing light
      PwrDownSt = 0;
      PD_Cnt = Pop[SPDT].Val*60;                              // Reset sleep timer
    }
  }
  return 0;
  //****************************************************************************
}

/******************************** END OF FILE *********************************/
