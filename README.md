## What is it?

Collection of some [OpenOCD](http://openocd.org/) snippets.
Usually it's ```connect_*.cfg``` files to handle different debuggers and some target specific commands.

## OpenOCD cheatsheet

### Run and connect

Specify directory with main openocd scripts and connect target.
P.S. Scripts directory can be also specified with ```OPENOCD_SCRIPTS``` environment variable.
```bash
openocd -s scripts -f connect_stlink-v2.cfg
```

Connect and open other ```.cfg``` with commands
```bash
openocd -f connect_stlink-v2.cfg -f ../generic/mass_erase.cfg
```

Connect and add some commands to command line, i.e. get target flash bank info and exit
```bash
openocd -f connect_stlink-v2.cfg -c "flash info 0" -c "exit"
```

Connect with ```telnet``` for typing commands in interactive mode
```bash
telnet localhost 4444
```

### Program

Program with ```program.bin``` from 0x00000000 address and then exit
```bash
program program.bin verify reset 0
```

Programming ```.hex``` or ```.elf``` file is almost the same - just remove start address
```bash
program program.hex verify reset
```

### Erase

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
