/********************* (C) COPYRIGHT 2018 e-Design Co.,Ltd. ********************
  FileName: FAT12.h
  Versions: DS213_APP Ver 1.0a                           Author : bure & SNAKE
*******************************************************************************/
#ifndef __FAT12_H
#define __FAT12_H

  #ifdef STM32F30X
    #include "stm32f30x.h"
  #elif STM32F10X_HD
    #include "stm32f10x.h"
  #endif

  typedef struct{
    u32 FAT1_BASE;          // FAT1Zone start address
    u32 FAT2_BASE;          // FAT2Zone start address
    u32 ROOT_BASE;          // Root directory start address
    u32 FILE_BASE;          // Start address of file area
    u32 FAT_LEN;
    u32 SEC_LEN;            // Sector length
    u32 FAT_END;            // End of link
    u8  FAT1_SEC;           // FAT1Number of sectors
    u8  FAT2_SEC;
  }FAT_ParamDef;

  //-------FLASH-----W25Q64BV---------------------------------------------------

  #define FILE_BASE_8M    0x7000     /*Start address of file area */
  #define ROOT_BASE_8M    0x3000     /* Root directory start address*/
  #define FAT_LEN_8M      0x1000
  #define FAT1_BASE_8M    0x1000     /* FAT1Zone start address*/
  #define FAT2_BASE_8M    0x2000     /* FAT2Zone start address*/
  #define SEC_LEN_8M      0x1000     /* Sector length */
  #define FAT1_SEC_8M     0x1        /* FAT1Number of sectors*/
  #define FAT2_SEC_8M     0x1        /* FAT2Number of sectors*/
  #define FAT_END_8M      0x7FF      /* End of link*/

  #define OK              0          /* Operation complete*/
  #define SEC_ERR         1          /* Sector read and write error*/
  #define FAT_ERR         2          /* FATTable read and write error*/
  #define OVER            3          /* Operation overflow*/
  #define NEW             4          /* Blank/new catalog item*/
  #define SUM_ERR         6          /* Checksum error*/

  #define OW              0          /* Or write (data is changed from 0 to 1)*/
  #define RW              1          /* Rewrite*/

  extern u8 Clash;
  extern FAT_ParamDef  FAT_Param;

  void InitFatParam(void);
//u8   ReadFileSec(u8 *Buffer);
//u8   ProgFileSec(u8 *Buffer);
//u8   OpenFileRd(u8 *Buffer, u8 *FileName);
//u8   OpenFileWr(u8 *Buffer, u8 *FileName);
//u8   CloseFile(u8 *Buffer, u32 Lenght);
u8   NextCluster(u16 *Cluster);
u8   ReadFileSec(u8 *pBuffer, u16 *pCluster);
u8   ProgFileSec(u8 *Buffer, u16 *Cluster);
//u8   ProgDiskPage(u8 *Buffer, u32 ProgAddr);
u8   SeekBlank(u8 *Buffer, u16 *Cluster);
u8   SetCluster(u8 *Buffer, u16 *Cluster);
u8   OpenFileRd(u8 *Buffer, u8 *FileName, u16 *Cluster, u32 *pDirAddr);
u8   OpenFileWr(u8 *Buffer, u8 *FileName, u16 *Cluster, u32 *pDirAddr);
u8   CloseFile(u8 *Buffer, u32 Lenght, u16 *Cluster, u32 *pDirAddr);

#endif
/********************************* END OF FILE ********************************/
