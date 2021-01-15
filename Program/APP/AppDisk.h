/**********************(C) COPYRIGHT 2018 e-Design Co.,Ltd.*********************
  AppDisk.h for DS213 APP                                        Author : bure
*******************************************************************************/
#ifndef __APP_DISK_H
#define __APP_DISK_H

  #ifdef STM32F30X
    #include "stm32f30x.h"
  #elif STM32F10X_HD
    #include "stm32f10x.h"
  #endif

  void AppDiskInit(void);
  void ProgDiskSect(u8 *pBuf, u32 DiskAddr);
  void ReadDiskData(u8 *pBuf, u32 DiskAddr, u16 n);
  void Write_Memory(u32 W_Offset, u32 W_Length);
  void Read_Memory (u32 R_Offset, u32 R_Length);

#endif
/*********************************  END OF FILE  ******************************/

