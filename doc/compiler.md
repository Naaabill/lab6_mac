# Cross-compiler

The target is a ST Nucleo32 board STM32F303K8.

## Required tools

The compiler suite is arm-none-eabi-gcc and the debugger is arm-none-eabi-gdb (that can be used in command line interface, or through a gui like QtCreator).

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
