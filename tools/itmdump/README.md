# itmdump
## Description

Simple utility to parse and dump ARM Cortex-M SWO trace output.
It is modified version of [OpenOCD](http://openocd.org/) ```contrib/itmdump.c```.
Changes were done special for using with QtCreator as an External Tool.

* All messages redirected to sdterr
* Char-by-char out replaced with buffered out

## Example of use
Print only strings captured:
```bash
itmdump -f file -d1
```
where ```file``` can be a regular file, named pipe or VCP (e.g. /dev/ttyUSB1).
Remove ```-d1``` argument to print raw ITM messages.
