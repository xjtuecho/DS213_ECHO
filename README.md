# DS213_ECHO

This is a customized DS213 APP firmware with Roll Mode support. Based on the official app
firmware v1.27, and fixed some bugs.

## What is [Roll Mode](https://www.sweetwater.com/insync/roll-mode/)?

On an oscilloscope, roll mode is a method of displaying acquired waveform data without
waiting for the complete waveform record. For example, if a sweep was 10 divisions long
with a rate of one second per division, and roll mode wasn’t available, it would take 10
seconds to fill the waveform record. But using roll mode, the oscilloscope will immediately
begin displaying results rather than waiting the full 10 seconds.

## Video Demo

- [Youtube](https://youtu.be/iGE-QNMqRk0)
- [Bilibili](https://www.bilibili.com/video/BV1QA411x7YX)

## Toolchain

The firmware is built on GNU Arm Embedded Toolchain. Both Windows and Linux are tested.

###  on Windows:

```
arm-none-eabi-gcc --version
arm-none-eabi-gcc.exe (GNU Tools for Arm Embedded Processors 9-2019-q4-major) 9.
2.1 20191025 (release) [ARM/arm-9-branch revision 277599]
Copyright (C) 2019 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```
### on Linux(Ubuntu 18.04):

```
arm-none-eabi-gcc --version
arm-none-eabi-gcc (15:6.3.1+svn253039-1build1) 6.3.1 20170620
Copyright (C) 2016 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

## FLASH Layout

The MCU of DS213 is STM32F103VET6, with 512kB(0x80000) FLASH. The FLASH layout is as follows:

| Name | Start      | Length  |
|:----:|:----------:|:--------|
| DFU  | 0x08000000 | 0x08000 |
| APP  | 0x08008000 | 0x68000 |
| FPGA | 0x08070000 | 0x0D800 |
| LOGO | 0x0807D800 | 8310    |

## Firmware Update Procedure

1. Enter DS213 DFU mode.
2. Copy FPGA_CFG.ADR to DFU disk, Then the DFU disk will restart.
3. Copy STMU_213_018.bin to DFU disk, Then the DFU disk will restart.
4. Copy DS213_ECHO_v20.12.25.hex to DFU disk.
5. Restart DS213.

There are no address information in the .bin file, so we put a .ADR file to DFU disk to setup the firmware start address.
That's what step 2 did. However, the .hex file contains address information, we don't need a .ADR file.
If your DS213 is using the official latest firmware, the FPGA is latest 018 version, you can omit step 2 and 3.
