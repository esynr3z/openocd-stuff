## What is it?

Collection of some [OpenOCD](http://openocd.org/) snippets and tools.
Usually it's ```connect_*.cfg``` files to handle different debuggers and some target specific commands.

## OpenOCD cheatsheet

### Run and connect

Specify directory with main openocd scripts and connect target.
P.S. Scripts directory can be also specified with ```OPENOCD_SCRIPTS``` environment variable.
```bash
openocd -s scripts -f connect_stlink.cfg
```

Connect and open other ```.cfg``` with commands
```bash
openocd -f connect_stlink.cfg -f ../generic/mass_erase.cfg
```

Connect and add some commands to command line, i.e. get target flash bank info and exit
```bash
openocd -f connect_stlink.cfg -c "flash info 0" -c "exit"
```

Connect with ```telnet``` for typing commands in interactive mode
```bash
telnet localhost 4444
```

### Flash

Program with ```program.bin``` from 0x00000000 address and then exit
```bash
program program.bin verify reset 0
```

Programming ```.hex``` or ```.elf``` file is almost the same - just remove start address
```bash
program program.hex verify reset
```

Erase target flash pages from 0 to 3
```bash
flash erase_sector 0 0 0
```

Mass erase target flash
```bash
flash erase_sector 0 0 last
```

### Memory access

Read 32bit word from address ```0x20000000```
```bash
mdw 0x20000000
```

Read 4 32bit word from address ```0x20000000```
```bash
mdw 0x20000000 4
```

Write 32bit word to address ```0x20000000```
```bash
mww 0x20000000 0xDEADBEEF
```

Get 1kB dump from ```0x00000000``` and save it to ```dump.bin```
```bash
dump_image dump.bin 0x00000000 0x400
```

Load binary file to memory from ```0x20000000```
```bash
load_image file.bin 0x20000000
```

### Trace

Set TRACECLKIN=12000000 Hz and config TPIU to use SWO with NRZ (uart) coding.
Messages will be saved to itm.fifo (can be file or pipe). Trace speed is set to maximum debugger can achieve.
```bash
tpiu config internal /tmp/itm.fifo uart off 72000000
```

Configure TPIU to let user capture trace output externally (with an additional UART or logic analyzer hardware).
Trace speed is set to 115200.
```bash
tpiu config external uart off 72000000 115200
```

Enable trace stimulus port 0 (default for ITM_SendChar() CMSIS function)
```bash
itm port 0 1
```

### GDB

If you want to send commands from GDB just add ```monitor``` prefix
```bash
monitor reset halt
monitor tpiu config internal /tmp/itm.fifo uart off 72000000
```
