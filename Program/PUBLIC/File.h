/********************* (C) COPYRIGHT 2018 e-Design Co.,Ltd. ********************
  File.h Ver: 2.0                                                Author : bure
*******************************************************************************/
#ifndef __FILE_H
#define __FILE_H

  #ifdef STM32F30X
    #include "stm32f30x.h"
  #elif STM32F10X_HD
    #include "stm32f10x.h"
  #endif

  #define FLASH_WAIT_TIMEOUT      100000
  #define PAGE_0                  0
  #define PAGE_1                  1
  #define PAGE_2                  2
  #define PAGE_3                  3

  #define OK                      0 // Operation complete
  #define SEC_ERR                 1 // Sector read and write error
  #define FAT_ERR                 2 // FAT table read and write error
  #define OVER                    3 // Operation overflow
  #define NEW                     4 // Blank/new catalog item
  #define EMPT                    4 // file does not exist
  #define VER_ERR                 5 // Wrong version
  #define SUM_ERR                 6 // Checksum error
  #define RD_ERR                  8 // Sector read error
  #define WR_ERR                  9 // Sector write error
  #define DISK_ERR                10 // Disk error

  extern uc8 F_NAME[][12];
  extern u8  TrckBuf[];
  u8  Snapshot(void);
  u8  SaveParam(void);
  u8  LoadParam(void);
  u8  SaveBuf(u8 FileNum);
  u8  SaveCsv(u8 FileNum);
  u8  SaveTrack(u8 FileNum);
  u8  LoadTrack(u8 FileNum);
  u16 SeekMaxFileNum(u8 *pName);

#endif
/********************************* END OF FILE ********************************/
