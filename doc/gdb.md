# Start debugging with GDB

## Introduction
The [GNU GDB debugger](https://www.gnu.org/software/gdb/) is the tool used to interact with the micro-controller on the board.
For low-level operations, it uses the ST-Link tool, dedicated to ST microcontrollers.

You can easily find documentation for GDB, from simple [cheat sheets](https://darkdust.net/files/GDB%20Cheat%20Sheet.pdf), [more extended doc](http://www.yolinux.com/TUTORIALS/GDB-Commands.html), and obviously the [official documentation](https://sourceware.org/gdb/current/onlinedocs/gdb/).

You have to plug your development board, and start in a terminal `st-util`
```sh
$ st-util
st-util 1.4.0
2018-11-23T17:08:25 INFO src/common.c: Loading device parameters....
2018-11-23T17:08:25 INFO src/common.c: Device connected is: F334 device, id 0x10016438
2018-11-23T17:08:25 INFO src/common.c: SRAM size: 0x3000 bytes (12 KiB), Flash: 0x10000 bytes (64 KiB) in pages of 2048 bytes
2018-11-23T17:08:25 INFO src/gdbserver/gdb-server.c: Chip ID is 00000438, Core ID is  2ba01477.
2018-11-23T17:08:25 INFO src/gdbserver/gdb-server.c: Listening at *:4242...
```
The `st-util` tool communicates with the GDB debugger through a socket (port 4242 in this case).

## Basic usage
### Start GDB
To start with the `text user interface` (tui), which is easier to start, with the binary file `test.elf`:

```sh
arm-none-eabi-gdb -tui test.elf
```
#### Startup

A gdb script is provided to initiate communication with the target, load the binary file and run until main. The content of the script is:
```
#start st-util from another terminal before running gdb:
#arm-none-eabi-gdb -tui test.elf
tar extended-remote :4242
load
break main
#continue (until main)
c
```
To execute this script, we just use the command in the gdb prompt:
```
source init.gdb
```


#### Layout
* `layout src`: same as the `-tui` CLI argument. Display src
* `layout asm`: display assembly

The text window is split in 2 parts, the source code file, and the command file. Most useful shortcuts are:

* Ctrl+X O //toggle focus between the 2 windows.
* Ctrl+X 2 //layout with 3 parts, including asm

### Run/stop
Basic commands are:
* `c` (continue) resumes the execution.
* `s` (step) - execute one instruction, **step into** function
* `n` (next) - execute one line, **step over** functions.
The execution can be stopped with `Ctrl+C`.

### Breakpoints
#### declare breakpoint
You can insert a breakpoint using the source code location, or the memory address:
```
b *0x8000502
```

insert a breakpoint at line 26 in file `test.c`:
```
b test.c:26
```
Insert a breakpoint at line 6 of the current C file:
```
b 6
```

#### list breakpoints
```
info breakpoints
```
#### Remove breakpoint
Remove breakpoint 1 (the id is given from the previous command)
```
delete 1
```
delete all breakpoints
```
delete
```
### Variables
#### View
Following commands can be used either with `print` (show one time) or `display` (show each time the program is stopped):

Print a variable one time (`/x` => hexadecimal display)
```
print/x i
```
Or from an address:
```
print/x *0x80004d8
```
Access to a structure (as in C):
```
print/x tim->CNT
```

Examine memory: `4` units, `x` => hexa, `w` => unit is word (4 bytes)
```
x/4xw *0x20002ff8
```
or (get the 4 words above the stack pointer)
```
x/4xw $sp
```

Get the value of the General Purpose Registers (GPR)
```
info registers
```

### Special case for the STM32 peripherals
the peripheral address is computed by the preprocessor from different `#define` commands. To get an access with gdb, we have to declare it in the C file:
```c
volatile GPIO_TypeDef * __attribute__((unused)) gpioa=GPIOA;
```
then, the access is easy in GDB:
```
print/x gpioa->MODER
```

### Update
We use the `set` keyword, example:
```
set gpioa->MODER = gpioa->MODER | 0xa0
```
