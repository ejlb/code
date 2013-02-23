#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "nark.h"

void
usage (char *program)
{
    // Narks command line options

    printf ("%s-%s - %s\n", program, VERSION, AUTHOR);
    printf ("%s [options]\n\n", program);
    printf ("  -i  --interface  \t - specify an interface to sniff on\n");
    printf ("  -f  --filter     \t - filter packets that match a pcap regex\n");
    printf ("  -L  --xml-log    \t - save a log of data in xml format\n");
    printf ("  -v  --verbose    \t - verbose output\n");
    printf ("  -h  --help       \t - this help message\n");
    printf ("\n");

    printf ("  -r  --enable-resolve      \t - enable host name resolving\n");
    printf ("  -n  --disable-portname  \t - disable port name conversions\n");
    printf ("  -p  --disable-promisc   \t - disable promiscious mode\n");
    printf ("\n");
    exit (0);
}
