ELF x86 32bit appender virus in 800b
====================================

The virus infects every infectable elf executable in the current directory 
using Silvio's classic .text section padding technique.

* mmap's host for improved efficiency 
* gets virus length using jmp trick so no bootstrapping
* overwrites ELF header entry point

'make' to compile the virus and an empty test host. Requires gcc and gas. 
'make debug' for a version that includes symbols
