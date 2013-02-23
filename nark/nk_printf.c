#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "nark.h"

// nark printf wrapper - not used for anything yet 
// but could be used in the future

int
nk_printf (char *format, ...)
{
    va_list ap;
    int len;
    char buf[BUFSIZ];

    va_start (ap, format);
    vsnprintf (buf, BUFSIZ, format, ap);

    len = printf ("%s", buf);

    va_end (ap);
    return len;
}
