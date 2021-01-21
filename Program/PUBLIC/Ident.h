/******************** (C) COPYRIGHT 2018 e-Design Co., Ltd. ********************
 Ident.h Ver 2.0                                        Author : bure
********************************************************************************

 * The hardware-related macro definitions that need to be provided externally when calling are as follows：

   #define TYPE_DS201 / TYPE_DS203 / ...   Product number
   #define LIC_REC      0x08xxxxxx         Licence Storage address

*******************************************************************************/
#ifndef __IDENT_H
#define __IDENT_H

  #ifdef STM32F30X
    #include "stm32f30x.h"
  #elif STM32F10X_HD
    #include "stm32f10x.h"
  #endif

  #define TYPE_LA104    // Temporarily borrow the Lic unlocking identification number of LA104
//  #define TYPE_DS213

  #define LIC_REC        0x080000FC

 // The functions that can be called externally with a uniform name are as follows：

  u32 Dev_SN(u32 Base);
  u32 AnswerCode(void);
  u8  CheckLic(u32 SN_Base);
  u8  CompareLic(u32 Lic_No, u32 SN_Base);
  u32 UserLicGen(u32 User_ID, u32 SN_Base, u32 Proj_ID);

#endif

/********************************* END OF FILE ********************************/
