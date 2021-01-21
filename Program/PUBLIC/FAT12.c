/**********************(C) COPYRIGHT 2018 e-Design Co.,Ltd.*********************
  FAT12.c Ver 2.0                                        Author : bure & SNAKE
*******************************************************************************/
#include "FAT12.h"
#include "Sys.h"

//u8  NextCluster();
//u8  SetCluster(u8 *Buffer);
//u8  SeekBlank(u8 *Buffer);

u8  Clash;
//u16 Cluster[3];
//u32 DirAddr;
FAT_ParamDef  FAT_Param;

/*******************************************************************************

*******************************************************************************/
void InitFatParam(void)
{
  FAT_Param.FAT1_BASE = FAT1_BASE_8M;
  FAT_Param.FAT2_BASE = FAT2_BASE_8M;
  FAT_Param.ROOT_BASE = ROOT_BASE_8M;
  FAT_Param.FILE_BASE = FILE_BASE_8M;
  FAT_Param.FAT1_SEC  = FAT1_SEC_8M;
  FAT_Param.FAT2_SEC  = FAT1_SEC_8M;
  FAT_Param.SEC_LEN   = SEC_LEN_8M;
  FAT_Param.FAT_LEN   = FAT_LEN_8M;
  FAT_Param.FAT_END   = FAT_END_8M;
}
/*******************************************************************************
 * FunctionName : NextCluster
 * Description  : Find the next linked cluster number and return. The current cluster number is stored at the pointer +1
 * Param        : u16* pCluster
 * Return       : u8
*******************************************************************************/
u8 NextCluster(u16 *pCluster)
{
    u16 FatNum;
    u32 Addr;

    Addr = FAT_Param.FAT1_BASE + (*pCluster + *pCluster / 2);

    *(pCluster + 1) = *pCluster;                                   // Save the previous cluster number
    *pCluster = 0;
    if ((*(pCluster + 1) >= FAT_Param.FAT_END) || (*(pCluster + 1) < 2)) return SEC_ERR;
//  if (ReadDiskData((u8 *)&FatNum, Addr, 2) != OK) return SEC_ERR;
    ReadDiskData((u8 *)&FatNum, Addr, 2);
    *pCluster = (*(pCluster + 1) & 1) ? (FatNum >> 4) : (FatNum & 0xFFF); //Point to the next cluster number
    return OK;
}

/*******************************************************************************
 * FunctionName : ReadFileSec
 * Description  : Read file sector(512 Bytes), When returning, the pointer points to the next cluster number,
 *                The current cluster number is stored at the position of pointer +1
 * Param        : u8* pBuffer
 * Param        : u16* pCluster
 * Return       : u8
*******************************************************************************/
u8 ReadFileSec(u8 *pBuffer, u16 *pCluster)
{
    u32 ReadAddr = FAT_Param.FILE_BASE + FAT_Param.SEC_LEN * (*pCluster - 2);

//  if (ReadDiskData(pBuffer, ReadAddr, FAT_Param.SEC_LEN) != OK) return SEC_ERR;
    ReadDiskData(pBuffer, ReadAddr, FAT_Param.SEC_LEN);
    if (NextCluster(pCluster) != 0) return FAT_ERR;                 // Remove a cluster number
    return OK;
}

/*******************************************************************************
 * FunctionName : ProgFileSec
 * Description  : Write file sector (512/4096 Bytes), fill in the current FAT table and return the next cluster number found
 * Param        : u8* pBuffer
 * Param        : u16* pCluster
 * Return       : u8
*******************************************************************************/
u8 ProgFileSec(u8 *pBuffer, u16 *pCluster)
{
    u16 Tmp;
    u32 ProgAddr = FAT_Param.FILE_BASE + FAT_Param.SEC_LEN * (*pCluster - 2);
//  if (ProgDiskPage(pBuffer, ProgAddr) != OK) return SEC_ERR;
    ProgDiskSect(pBuffer, ProgAddr);
    if (NextCluster(pCluster) != 0) return FAT_ERR;                 // Remove a cluster number
    Tmp = *(pCluster + 1);
    if (*pCluster == 0)
    {
        *pCluster = Tmp;
        if (SeekBlank(pBuffer, pCluster) != OK) return OVER;
        if (SetCluster(pBuffer, pCluster) != OK) return SEC_ERR;
    }
    return OK;
}

/*******************************************************************************
 * FunctionName : SeekBlank
 * Description  : Find the free cluster number, when returning, the pointer points to the next free cluster number, and the current cluster number is stored at the position of pointer +1
 * Param        : u8* pBuffer
 * Param        : u16* pCluster
 * Return       : u8
*******************************************************************************/
u8 SeekBlank(u8 *pBuffer, u16 *pCluster)
{
    u16  Tmp;
    u8   Buffer[2];
    u8   Tmp_Flag = 1;

    *(pCluster + 1) = *pCluster;                                    // Save the current cluster number

    for (*pCluster = 0; (*pCluster) < 4095; (*pCluster)++)
    {
//      if (ReadDiskData(Buffer, FAT_Param.FAT1_BASE + (*pCluster) + (*pCluster) / 2, 2) != 0) return SEC_ERR;
        ReadDiskData(Buffer, FAT_Param.FAT1_BASE + (*pCluster) + (*pCluster) / 2, 2);
        Tmp = ((*pCluster) & 1) ? ((*(u16 *)Buffer) >> 4) : ((*(u16 *)Buffer) & 0xFFF);

        if ((Tmp == 0) && (Tmp_Flag == 0) && (((*pCluster)) != *(pCluster + 1)))
        {
            Tmp_Flag = 1;
            return OK;
        }
        if ((Tmp == 0) && (Tmp_Flag == 1))
        {
            *(pCluster + 2) = *pCluster;
            Tmp_Flag = 0;
        }
    }
    return OK;
}

/*******************************************************************************
 * FunctionName : SetCluster
 * Description  : Write the next cluster number into the current cluster link bit of the FAT table, and the pointer points to the next cluster number when returning,
 *                Pointer +1 is the current cluster number
 * Param        : u8* pBuffer
 * Param        : u16* pCluster
 * Return       : u8
*******************************************************************************/
u8 SetCluster(u8 *pBuffer, u16 *pCluster)
{
    u16  Offset, i, k;
    u32  SecAddr;

    i = *(pCluster + 1);                  // Extract the original current cluster number
    k = *pCluster;                        // Extract the next cluster number
    Offset = i + i / 2;
    SecAddr = FAT_Param.FAT1_BASE + (Offset & 0xF000);
    Offset &= 0x0FFF;
//  if (ReadDiskData(pBuffer, SecAddr, FAT_Param.SEC_LEN) != 0) return SEC_ERR;
    ReadDiskData(pBuffer, SecAddr, FAT_Param.SEC_LEN);
    if (i & 1)
    {
        pBuffer[Offset] = (pBuffer[Offset] & 0x0F) + ((k << 4) & 0xF0);
        pBuffer[Offset + 1] = k >> 4;
    } else
    {
        pBuffer[Offset] = k & 0xFF;
        pBuffer[Offset + 1] = (pBuffer[Offset + 1] & 0xF0) + ((k >> 8) & 0x0F);
    }

//  if (ProgDiskPage(pBuffer, SecAddr) != 0) return SEC_ERR;
    ProgDiskSect(pBuffer, SecAddr);
    return OK;
}

/*******************************************************************************
 * FunctionName : OpenFileRd
 * Description  : Open the file in read mode: return the first cluster number and directory entry address of the file
 *                Or 0 cluster number and the address of the first blank directory entry
 * Param        : u8* pBuffer
 * Param        : u8* pFileName
 * Param        : u16* pCluster
 * Param        : u32* pDirAddr
 * Return       : u8
*******************************************************************************/
u8 OpenFileRd(u8 *pBuffer, u8 *pFileName, u16 *pCluster, u32 *pDirAddr)
{
    u16 i, n;

    *pCluster = 0;
    for (*pDirAddr = FAT_Param.ROOT_BASE; *pDirAddr < FAT_Param.FILE_BASE;)
    {
//      if (ReadDiskData(pBuffer, *pDirAddr, FAT_Param.SEC_LEN) != OK) return SEC_ERR;
        ReadDiskData(pBuffer, *pDirAddr, FAT_Param.SEC_LEN);
        for (n = 0; n < FAT_Param.SEC_LEN; n += 32)
        {
            for (i = 0; i < 11; i++)
            {
                if (pBuffer[n + i] != 0)
                {
                    if (pBuffer[n + i] != pFileName[i]) break;
                    if (i == 10)                                  // Find file name
                    {
                        *pCluster = *(u16 *)(pBuffer + n + 0x1A); // File first cluster number
                        return OK;
                    }
                } else return NEW;               // Return after encountering the first blank directory entry
            }
            *pDirAddr += 32;
        }
    }
    return OVER;
}

/*******************************************************************************
 * FunctionName : OpenFileWr
 * Description  : Open the file in writing mode: return the first cluster number and directory entry address of the file
 * Param        : u8* pBuffer
 * Param        : u8* pFileName
 * Param        : u16* pCluster
 * Param        : u32* pDirAddr
 * Return       : u8
*******************************************************************************/
u8 OpenFileWr(u8 *pBuffer, u8 *pFileName, u16 *pCluster, u32 *pDirAddr)
{
    u32 i, n, offset;

    i = OpenFileRd(pBuffer, pFileName, pCluster, pDirAddr);
    if (i != NEW) return i;
    else                                                     // The current item is a blank directory item
    {
        if (SeekBlank(pBuffer, pCluster) != OK) return OVER; // If the FAT table is full, return
        n = *pDirAddr & 0xFFF;
        offset = *pDirAddr - n;
//      if (ReadDiskData(pBuffer, offset, FAT_Param.SEC_LEN) != OK) return SEC_ERR;
        ReadDiskData(pBuffer, offset, FAT_Param.SEC_LEN);
        for (i = 0; i < 11; i++) pBuffer[n + i] = pFileName[i]; // Create new catalog itemoffset +
        *(u16 *)(pBuffer + n + 0x1A) = *pCluster;
//      if (ProgDiskPage(pBuffer, offset) != OK) return SEC_ERR;
        ProgDiskSect(pBuffer, offset);
        return OK;
    }
}

/*******************************************************************************
 * FunctionName : CloseFile
 * Description  : Close the file and write the terminator to the FAT table，
 *                Write the file length into the directory entry, copy FAT1 to FAT2
 * Param        : u8* pBuffer
 * Param        : u32 Lenght
 * Param        : u16* pCluster
 * Param        : u32* pDirAddr
 * Return       : u8
*******************************************************************************/
u8 CloseFile(u8 *pBuffer, u32 Lenght, u16 *pCluster, u32 *pDirAddr)
{
    u32 n;
    n = 0xFFF;
    *pCluster = 0xFFF;
    SetCluster(pBuffer, pCluster);

//  if (ReadDiskData(pBuffer, (*pDirAddr & (~n)), FAT_Param.SEC_LEN) != OK) return SEC_ERR;
    ReadDiskData(pBuffer, (*pDirAddr & (~n)), FAT_Param.SEC_LEN);
    *(u8 *)(pBuffer + (*pDirAddr & n) + 0x0B) = 0x20;
    *(u32 *)(pBuffer + (*pDirAddr & n) + 0x1C) = Lenght;
//  if (ProgDiskPage(pBuffer, (*pDirAddr & (~n))) != OK) return SEC_ERR;
    ProgDiskSect(pBuffer, (*pDirAddr & (~n)));
//  if (ReadDiskData(pBuffer, FAT_Param.FAT1_BASE, FAT_Param.SEC_LEN) != OK) return SEC_ERR;
    ReadDiskData(pBuffer, FAT_Param.FAT1_BASE, FAT_Param.SEC_LEN);
//  if (ProgDiskPage(pBuffer, FAT_Param.FAT2_BASE) != OK) return SEC_ERR;
    ProgDiskSect(pBuffer, FAT_Param.FAT2_BASE);
    return OK;
}
/*********************************  END OF FILE  ******************************/