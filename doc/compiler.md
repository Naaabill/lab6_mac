# Cross-compiler

The target is a ST Nucleo32 board STM32F303K8.

## Required tools

The compiler suite is arm-none-eabi-gcc, the debugger arm-none-eabi-gdb (that can be used in command line interface, or through a gui like QtCreator).

### Linux
On Linux (Debian/Ubuntu flavor), you should install:
 * `build-essential`
 * [st-link](https://github.com/texane/stlink)

First the essential build tools:
```sh
sudo apt-get install build-essential
```

The compiler can be downloaded through packages, but there are some problems with it. You can download [a version provided by ARM](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads).

The archive may be decompressed, and the environment variable PATH should be updated (here if the archive has been decompressed to `/path/to/gcc-arm`):
```sh
export PATH=/path/to/gcc-arm/bin:$PATH
```

This environment variable should be defined each time a terminal is opened, so you can add it in the file `~/.bashrc`, which is a script that is executed each time a terminal is opened on Linux.

A recent version of the [CMake build process software](https://cmake.org/download/) should be installed.

### Mac OS

On Mac, XCode should be installed, with the command line tools. You should agree the license in terminal (it has already been done on computers of the lab):
```sh
sudo xcodebuild -license
```
You can [download the compiler here.](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads).

The ST-Link tool (communication between the debugger gdb and the target) [can be found here](https://github.com/texane/stlink).

The archive of both the compiler and stlink may be decompressed, and the environment variable PATH should be updated (here if the archive has been decompressed to `/path/to/gcc-arm`):
```sh
export PATH=/path/to/gcc-arm/bin:$PATH
export PATH=/path/to/stlink/bin:$PATH
```

This environment variable should be defined each time a terminal is opened, so you can add it in the file `~/.profile`, which is a script that is executed each time a terminal is opened on Mac.

A recent version of the [CMake build process software](https://cmake.org/download/) should be installed.

## Windows

(not tested yet…)

You should install:
 * [st-link](https://github.com/texane/stlink) => version compilée [64 bits](https://github.com/stlink-org/stlink/releases/download/v1.6.1/stlink-1.6.1-x86_64-w64-mingw32.zip) et [32 bits](https://github.com/stlink-org/stlink/releases/download/v1.6.1/stlink-1.6.1-i686-w64-mingw32.zip).
 * the cross-compiler [gcc for ARM](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads)
 * the [CMake build process software](https://cmake.org/download/)
