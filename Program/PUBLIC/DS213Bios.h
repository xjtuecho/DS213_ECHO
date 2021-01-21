/********************* (C) COPYRIGHT 2018 e-Design Co.,Ltd. ********************
  Bios.h for DS213 DFU & APP                                     Author : bure
*******************************************************************************/
#ifndef __BIOS_H
#define __BIOS_H

  #ifdef STM32F30X
    #include "stm32f30x.h"
  #elif STM32F10X_HD
    #include "stm32f10x.h"
  #endif

  #include <stdlib.h>
  #include <string.h>
  #include "Func.h"
  #include "Ident.h"
  #include "USB_bot.h"
  #include "usb_lib.h"
  #include "USB_pwr.h"
  #include "DS213HwDriver.h"

  extern HwDrvDef  Hw;

  void __BiosInit(u32 pDrv);
  void SetLicence(u16 x, u16 y);

  //-------------------- DevInit(u16 Dev)Dev macro definition in the function ---------------------

  enum {
    PWRCTRL, // Power control equipment
    KEYnDEV, // Key input device
    BUZZDEV, // Buzzer device
    DISPDEV, // LCD display screen
    USB_DEV, // USB communication device
    USB_SCI, // SCI communication device
    USBDISK, // USB Disk equipment
    FPGADEV, // FPGA Interface device
    FPGACFG, // FPGA Power-on configuration
    SGNL_IN, // Signal input channel
    SGNLOUT, // Signal output channel
    SO_ANLG, // Analog signal output
    SO_DGTL, // Digital signal output
  };

//-------------------------------- Display related definitions --------------------------------

  #define WHT        0xFFFF
  #define CYAN       0xFFE0
  #define YEL        0x07FF
  #define PRPL       0xF81F
  #define GRN        0x07E0
  #define GRAY       0x7BEF
  #define DARK       0x39E7
  #define BLUE       0xF800
  #define ORNG       0x051F
  #define RED        0x001F
  #define BLK        0x0000

  #define CHAR       0x00   //  Standard Display Character display
  #define INVR       0x01   //  Inverse Display  Reverse display
  #define SYMB       0x02   //  Symbol Display   Symbol display
  #define VOID       0x04   //  Dotted Display   Ghost display

#endif
/********************************* END OF FILE ********************************/
