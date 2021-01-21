/********************* (C) COPYRIGHT 2017 e-Design Co.,Ltd. ********************
  DS213_APP Fpga.h                                               Author : bure
*******************************************************************************/
#ifndef __FPGADRV_H
#define __FPGADRV_H


//=================== FPGA Control related definitions ===================

  #define A_C_CH         0
  #define B_D_CH         1

  //FPGA control commands
  #define CHIP_RST       0x80  /* FPGA Initial reset */


  //FPGA Definition of working status flag
  #define SMPL_Din       0x01    /* Digital signal readout   */
  #define PSMPL          0x02    /* Signal pre-sampling completed */
  #define TRIGG          0x04    /* Trigger conditions are met   */
  #define FULL           0x08    /* Data buffer is full */
  #define EMPTY          0x10    /* Data buffer read empty */
  #define TRIGG_ASK      0x20    /* Local trigger request   */
  #define SCRNF          0x40    /*Full screen of data     */

  //Sampling control parameter setting command
  #define SMPL_MODE      0x40    /* Sampling mode setting             */
  #define SMPL_PSCL      0x41    /* Sampling period prescaler   0~255   */
  #define SMPL_TDIV      0x42    /* Sampling period setting     0~65535 */
  #define SMPL_PSMP      0x43    /* Pre-sampling depth setting   0~4095  */
  #define SMPL_RPTR      0x45    /* Data read pointer setting   0~4095  */
  #define SMPL_RD        0x46    /* Sampling data and status reading 9 bits  */
  #define SMPL_ST        0x47    /* FPGA Status readout    6Bits  */

  //Sampling mode SMPL_MODE definition
  #define SEPARATE       0x00    /* ADC Parallel sampling */
  #define INTERLACE      0x01    /* ADC Interleaved sampling */

  //Trigger control parameter setting command
  #define TRIG_KIND      0x20    /* Trigger type setting      0~31     1 */
  #define TRIG_VOLT      0x21    /* Trigger level threshold      0~255  100 */
  #define TRIG_WDTH      0x22    /* Trigger pulse width threshold     0~4095  35 */

  //Measurement data readout selection definition
  #define VMIN           0x30    /* Simulated minimum 1B */
  #define VMAX           0x31    /* Simulated maximum 1B */
  #define VAVG           0x32    /* Simulated average 1B */
  #define ATWL           0x34    /* Analog signal negative pulse width*16      2B*1 */
  #define ATWH           0x35    /* Analog signal positive pulse width*16      2B*1 */
  #define DTWL           0x36    /*Digital signal negative pulse width*16      2B*1 */
  #define DTWH           0x37    /* Digital signal positive pulse width*16      2B*1 */

  //Trigger type TRIG_KIND parameter definition
  #define TRIG_NONE      0x00    /* Stop trigger               */
  #define TRIG_ANY       0x01    /* Arbitrary trigger               */

  #define TRIG_AiDN      0x10    /* Analog signal negative step trigger     */
  #define TRIG_AiUP      0x11    /* Analog signal positive step trigger     */
  #define TRIG_AiLT      0x12    /* Trigger when the analog level is less than the threshold   */
  #define TRIG_AiGT      0x13    /* Trigger on analog level greater than threshold   */
  #define TRIG_AnLT      0x14    /* Analog negative pulse width less than threshold trigger */
  #define TRIG_AnGT      0x15    /* Analog negative pulse width greater than threshold trigger */
  #define TRIG_ApLT      0x16    /* Analog positive pulse width less than threshold trigger */
  #define TRIG_ApGT      0x17    /* Analog positive pulse width greater than threshold trigger */

  #define TRIG_DiDN      0x18    /* Digital signal negative step trigger     */
  #define TRIG_DiUP      0x19    /*Digital signal positive step trigger     */
  #define TRIG_DiLT      0x1A    /*Digital signal low level trigger    */
  #define TRIG_DiGT      0x1B    /*Digital signal high level trigger     */
  #define TRIG_DnLT      0x1C    /* Digital negative pulse width less than threshold trigger */
  #define TRIG_DnGT      0x1D    /* Digital negative pulse width greater than threshold trigger */
  #define TRIG_DpLT      0x1E    /* Digital positive pulse width less than threshold trigger */
  #define TRIG_DpGT      0x1F    /* Digital positive pulse width greater than threshold trigger */

#endif
