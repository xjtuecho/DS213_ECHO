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
