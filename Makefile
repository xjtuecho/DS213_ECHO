##########################################################################################################################
# File automatically-generated by tool: [projectgenerator] version: [3.0.0] date: [Tue Apr 23 11:19:56 CST 2019]
##########################################################################################################################

# ------------------------------------------------
# Generic Makefile (based on gcc)
#
# ChangeLog :
#	2017-02-10 - Several enhancements + project update mode
#   2015-07-22 - first version
# ------------------------------------------------

######################################
# target
######################################
TARGET = DS213_ECHO


######################################
# building variables
######################################
# debug build?
DEBUG = 0
# optimization
OPT = -Og


#######################################
# paths
#######################################
# Build path
BUILD_DIR = build

######################################
# source
######################################
# C sources
C_SOURCES =  \
Program/PUBLIC/FAT12.c \
Program/PUBLIC/File.c \
Program/PUBLIC/Func.c \
Program/PUBLIC/Interrupt.c \
Program/APP/AppDisk.c \
Program/APP/Calibrat.c \
Program/APP/Draw.c \
Program/APP/Main.c \
Program/APP/Menu.c \
Program/APP/Process.c \
Program/APP/Sys.c \
Libraries/STM32_USB-FS-Device_Driver/src/usb_core.c \
Libraries/STM32_USB-FS-Device_Driver/src/usb_init.c \
Libraries/STM32_USB-FS-Device_Driver/src/usb_int.c \
Libraries/STM32_USB-FS-Device_Driver/src/usb_mem.c \
Libraries/STM32_USB-FS-Device_Driver/src/usb_regs.c \
Libraries/STM32_USB-FS-Device_Driver/src/usb_sil.c \
Program/MSD/USB_bot.c \
Program/MSD/USB_desc.c \
Program/MSD/USB_istr.c \
Program/MSD/USB_prop.c \
Program/MSD/USB_pwr.c \
Program/MSD/USB_scsi.c \
Libraries/STM32F10x_SPD/src/misc.c \
Libraries/STM32F10x_SPD/src/stm32f10x_adc.c \
Libraries/STM32F10x_SPD/src/stm32f10x_bkp.c \
Libraries/STM32F10x_SPD/src/stm32f10x_can.c \
Libraries/STM32F10x_SPD/src/stm32f10x_cec.c \
Libraries/STM32F10x_SPD/src/stm32f10x_dac.c \
Libraries/STM32F10x_SPD/src/stm32f10x_dbgmcu.c \
Libraries/STM32F10x_SPD/src/stm32f10x_dma.c \
Libraries/STM32F10x_SPD/src/stm32f10x_exti.c \
Libraries/STM32F10x_SPD/src/stm32f10x_flash.c \
Libraries/STM32F10x_SPD/src/stm32f10x_fsmc.c \
Libraries/STM32F10x_SPD/src/stm32f10x_gpio.c \
Libraries/STM32F10x_SPD/src/stm32f10x_i2c.c \
Libraries/STM32F10x_SPD/src/stm32f10x_iwdg.c \
Libraries/STM32F10x_SPD/src/stm32f10x_pwr.c \
Libraries/STM32F10x_SPD/src/stm32f10x_rcc.c \
Libraries/STM32F10x_SPD/src/stm32f10x_rtc.c \
Libraries/STM32F10x_SPD/src/stm32f10x_sdio.c \
Libraries/STM32F10x_SPD/src/stm32f10x_spi.c \
Libraries/STM32F10x_SPD/src/stm32f10x_tim.c \
Libraries/STM32F10x_SPD/src/stm32f10x_usart.c \
Libraries/STM32F10x_SPD/src/stm32f10x_wwdg.c \
Libraries/CMSIS/Device/ST/STM32F10x/Source/STM32F103_System.c

# ASM sources
ASM_SOURCES =  \
Libraries/CMSIS/Device/ST/STM32F10x/Source/startup_stm32f10x_hd.s

#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
#AS = $(GCC_PATH)/$(PREFIX)as
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
LD = $(GCC_PATH)/$(PREFIX)gcc
AR = $(GCC_PATH)/$(PREFIX)ar
else
CC = $(PREFIX)gcc
#AS = $(PREFIX)as
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
LD = $(GCC_PATH)gcc
AR = $(GCC_PATH)ar
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m3

# fpu
# NONE for Cortex-M0/M0+/M3

# float-abi


# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS =

# C defines
C_DEFS =  \
-DUSE_STDPERIPH_DRIVER \
-DSTM32F10X_HD


# AS includes
AS_INCLUDES =

# C includes
C_INCLUDES =  \
-IProgram/APP \
-IProgram/PUBLIC \
-IProgram/MSD \
-ILibraries/CMSIS/Include \
-ILibraries/CMSIS/Device/ST/STM32F10x/Include \
-ILibraries/STM32F10x_SPD/inc \
-ILibraries/STM32_USB-FS-Device_Driver/inc

# compile gcc flags
#ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections
ASFLAGS = $(MCU) $(C_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -Wno-pointer-sign -Wno-unused-variable -fdata-sections -ffunction-sections -funsigned-char

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = STM32F103VCT6_APP.ld

# libraries
#LIBS = -lc -lm -lnosys
LIBS = -lm -static
LIBDIR =
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(ASLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@

$(BUILD_DIR):
	mkdir $@

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)

#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***