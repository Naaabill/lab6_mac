# Setting up programming environment

This project contains the practical work for the MICRO course.
This file contains important information about the configuration of the host computer,
and some basics on the C compiler suite usage.

The target is a ST Nucleo32 board STM32F303K8. We assume that the host computer is either a Linux (tested with Ubuntu 18.04) or a Mac computer.

## Required tools

We do not use any IDE, but a C code editor (Vim, Emacs, Gedit, …).
The compilation and flash phases are based on a provided Makefile

The compiler suite is arm-none-eabi-gcc. The debugging is done using directly arm-none-eabi-gdb (or ddd for a GUI).

### Linux
On Linux (Debian/Ubuntu flavor), you should install:
 * `build-essential`
 * `gcc-arm-none-eabi`
 * `gdb-arm-none-eabi` (to debug)
 * [st-link](https://github.com/texane/stlink)

First the essential build tools:
```sh
sudo apt-get install build-essential
```

Ubuntu provides a compiler version, but no more the associated debugger! The best way is to use [the one provided by ARM](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads).
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

## Compile/Run

### Compile

To compile, just use the makefile:
```sh
make
```

### Flash

To flash the application (st-link should be installed first):

```sh
make flash
```

### Debug

to debug the application (using gdb), first run `st-util` in another terminal, and run:

```sh
arm-none-eabi-gdb -tui test.elf
```

a gdb script is provided to initiate communication with target, load the application and run until main. You  just hae to call the init script inside gdb:

```gdb
source init.gdb
```

see file [gdb.md](./gdb.md) to have basic command to start debug your application!

### Clean

To remove generated files (obj/exec):

```sh
make flash
```
