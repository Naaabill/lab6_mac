# Cross-compiler, CMake, ST-Link

The target is a ST Nucleo32 board STM32F303K8.

## Required tools

Here, we use:
 * a cross-compiler for the cortex ARM target (`arm-none-eabi-gcc`)
 * a tool to upload and debug on the target: `st-link`
 * a tool to handle the project (compilation/flash recipes): `cmake`
 * an IDE (QtCreator, Eclipse,…), or a lightweight editor (VSCode, Vim, gedit, …)

The compiler suite is `arm-none-eabi-gcc`, the debugger `arm-none-eabi-gdb` (that can be used in command line interface, or through a gui like QtCreator or VSCode). The name stands for:
 * `arm`: the processor target
 * `none`: the operating system. Here `none` means that there is no operating system (baremetal)
 * `eabi` is Embedded Application Binary Interface and defines the rules to interact with other binaries (how to give the function arguments in the asm for instance)
 * `gcc` is the name for the `GNU Compiler collection`, a widely uses compiler.

 Here are some tips and links to install these tools on Linux, MacOS and Windows.

### Linux
On Linux (Debian/Ubuntu flavor), you should install:
 * `build-essential`
 * [st-link](https://github.com/texane/stlink)
 * [cmake build process software](https://cmake.org/download/)

First the essential build tools and cmake
```sh
sudo apt-get install build-essential cmake
```

The compiler can be downloaded through packages, but there are some problems with it. You can download [a version provided by ARM](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads).

The archive may be decompressed, and the environment variable PATH should be updated (here if the archive has been decompressed to `/path/to/gcc-arm`):
```sh
export PATH=/path/to/gcc-arm/bin:$PATH
```

This environment variable should be defined each time a terminal is opened, so you can add it in the script executed each time a terminal is opened (`~/.bashrc` or `~/.zprofile` or …).

### Mac OS

On Mac, XCode should be installed, with the command line tools (App Store). You should agree the license in terminal (it has already been done on computers of the lab):
```sh
sudo xcodebuild -license
```
You can [download the compiler here.](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads).

Packages for cmake build process software [can be found here](https://cmake.org/download/)

The ST-Link tool (communication between the debugger gdb and the target) [can be found here](https://github.com/texane/stlink).

The archive of both the compiler and stlink may be decompressed, and the environment variable PATH should be updated (here if the archive has been decompressed to `/path/to/gcc-arm`):
```sh
export PATH=/path/to/gcc-arm/bin:$PATH
export PATH=/path/to/stlink/bin:$PATH
```

This environment variable should be defined each time a terminal is opened, so you can add it in the file `~/.profile`, which is a script that is executed each time a terminal is opened on Mac.

A recent version of the [CMake build process software](https://cmake.org/download/) should be installed.

## Windows

You should install:
 * [st-link](https://github.com/texane/stlink) => binary version [64 bits](https://github.com/stlink-org/stlink/releases/download/v1.6.1/stlink-1.6.1-x86_64-w64-mingw32.zip) and [32 bits](https://github.com/stlink-org/stlink/releases/download/v1.6.1/stlink-1.6.1-i686-w64-mingw32.zip).
 * the cross-compiler [gcc for ARM](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads)
 * the [CMake build process software](https://cmake.org/download/)

You should take care that tools are available in the environment configuration. [here](https://docs.oracle.com/en/database/oracle/machine-learning/oml4r/1.5.1/oread/creating-and-modifying-environment-variables-on-windows.html#GUID-DD6F9982-60D5-48F6-8270-A27EC53807D0) is an example of the configuration on Windows 10 (not tested).