# GNU toolchain template

This is a project template compatable with 01-template, which use arm-none-eabi-gcc and make as compile tools. 

这是一个使用GNU工具链(make和arm-gcc)进行编程的工程模板，目前由个人开发。这个模板以Weact工作室的例程01-template为基础，添加了arm-gcc编译器的支持，可以作为使用GNU工具链进行GD32系列单片机开发的参考。

## Compile

对于Linux系统，安装 make 和 arm-none-eabi-gcc，确保在终端中运行下列命令有输出

For Linux, make sure you could get someting like these in ternimal after execute the command

```
~$ make -v
GNU Make 4.2.1
Built for aarch64-unknown-linux-gnu
Copyright (C) 1988-2016 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.

~$ arm-none-eabi-gcc -v
Using built-in specs.
COLLECT_GCC=arm-none-eabi-gcc
Target: arm-none-eabi
Thread model: single
Supported LTO compression algorithms: zlib
gcc version 10.2.1 20201103 (release) (GNU Arm Embedded Toolchain 10-2020-q4-major)
```

对于Windows操作系统，需要安装 arm-none-eabi-gcc 并确保将它们添加到环境变量中，make 工具推荐使用 mingw64，在命令提示符中运行下列命令有正确输出

For Windows, install arm-none-eabi-gcc and mingw64(or msys, wsl, if you know how dose it work) and make sure you could get someting like these in ternimal after execute the command

```
> mingw32-make -v 

GNU Make 4.2.1
Built for x86_64-w64-mingw32
Copyright (C) 1988-2016 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.

> arm-none-eabi-gcc -v

Using built-in specs.
...
gcc version 9.2.1 20191025 (release) [ARM/arm-9-branch revision 277599] (GNU Tools for Arm Embedded Processors 9-2019-q4-major) 
```

安装完成后，在终端中进入工程模板所在的文件夹，执行 make 命令编译示例工程

Compile the example project

```
> cd \Github\BluePill-Plus\SDK\GD32F303CCT6\Examples\GNU-template
> mingw32-make

mkdir build
arm-none-eabi-gcc -c -mcpu=cortex-m4 -mthumb   -DGD32F30X_HD  -ICore/Include -IDrivers/GD32F30x_standard_peripheral/Include -IDrivers/CMSIS/GD/G  -Og -Wall -fdata-sections -ffunction-sections -g -gdwarf-2 -MMD -MP -MF"build/gd32f30x_it.d" -Wa,-a,-ad,-alms=build/gd32f30x_it.lst Core/Sourc0x_it.o
...
arm-none-eabi-size build/GNU-template.elf
   text    data     bss     dec     hex filename
  19000     476    1612   21088    5260 build/GNU-template.elf
arm-none-eabi-objcopy -O ihex build/GNU-template.elf build/GNU-template.hex
arm-none-eabi-objcopy -O binary -S build/GNU-template.elf build/GNU-template.bin
```

如果出现了这样的输出，说明编译成功，在/build文件夹下可以找到二进制文件下载到单片机。

You may find GNU-template.elf in ./build/ directory.

本模板具有与例程01-template一致的功能，包括LED闪烁、串口输出。对于本模板的任何问题，可以通过项目issue与开发者联系。

This project have the same function with 01-template, you could see LED blinking. If you have any question with this template, please contact me by adding a issue.

## Flash & Debug

Giga Device have provided [programming software](https://github.com/WeActTC/BluePill-Plus/tree/master/Soft/GD32), support ISP, DFU, and others. You could also use [OpenOCD](http://openocd.org/) for flashing and debugging, the configuration file for STM32F3  `stm32f3x.cfg`  is compatible with GD32F3.

## Advance

本部分将介绍如何修改模板，创建自己的项目。

### 工程模板

工程模板结构如下所示

```
GNU-template
	|----Core
	|     |----Include
	|     |----Source
	|----Drivers
	|     |----CMSIS
	|     |----GD32F30x_standard_peripheral
	|----Makefile
```

Core文件夹下是主要的工程代码，外设的配置和主程序位于这个文件夹当中，用户代码主要存放于此。Drivers文件夹是GD32单片机的固件库，不需要作修改。如果需要使用USB固件库，则应将USB固件库复制到Drivers文件夹当中。

### Makefile

Makefile文件描述了工程的编译规则，我们要根据自己项目的需求修改模板Makefile文件。

1. 工程名称和优化选项

   将TARGET选项修改为自己项目的名称，根据需求选择是否构建DEBUG工程和优化选项。

   ```makefile
   ######################################
   # target
   ######################################
   TARGET = GNU-template
   
   ######################################
   # building variables
   ######################################
   # debug build?
   DEBUG = 1
   # optimization
   OPT = -Og
   ```

2. 源代码

   将工程包含的所有源代码文件路径添加到这一部分，格式如下所示。工程模板的Makefile已经加入了固件库中所有的源代码文件，用户只需要补充自己的外设配置文件和USB库源代码文件（如果使用了USB库）即可。

   ```makefile
   ######################################
   # source
   ######################################
   # C sources
   C_SOURCES =  \
   Core/Source/gd32f30x_it.c \
   Core/Source/gpio.c \
   Core/Source/main.c \
   Core/Source/systick.c \
   Core/Source/usart.c \
   Drivers/GD32F30x_standard_peripheral/Source/gd32f30x_adc.c \
   Drivers/GD32F30x_standard_peripheral/Source/gd32f30x_bkp.c \
   Drivers/GD32F30x_standard_peripheral/Source/gd32f30x_can.c \
   ```

3. 头文件

   将所有的头文件目录添加到这一部分，格式如下所示。

   ```makefile
   # C includes
   C_INCLUDES =  \
   -ICore/Include \
   -IDrivers/GD32F30x_standard_peripheral/Include \
   -IDrivers/CMSIS/GD/GD32F30x/Include \
   -IDrivers/CMSIS \
   ```

4. HID_IAP

   Weact Studio提供的例程全部支持HID IAP功能。若要使用HID IAP功能，首先在Makefile中取消HID_IAP定义的注释：

   ```makefile
   # C defines
   # Uncomment -DHID_IAP if you want a HID IAP application
   C_DEFS =  \
   -DGD32F30X_HD \
   #-DHID_IAP \
   ```

   再更换使用的链接脚本，注释掉LDSCRIPT = GD32F303CCTx.ld，再将#LDSCRIPT = GD32F303CCTx_HID.ld取消注释

   ```makefile
   #######################################
   # LDFLAGS
   #######################################
   # link script
   LDSCRIPT = GD32F303CCTx.ld
   # Uncomment this if you want a HID IAP application 
   #LDSCRIPT = GD32F303CCTx_HID.ld
   ```

   修改完成后编译得到的二进制文件即可使用GD32 HID IAP软件进行升级。

