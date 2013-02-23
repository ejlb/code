#include <stdio.h>
#include <string.h>
#include "nark.h"

#define ADDR_BUF_MAX 20

int
print_mac (char *mac_addr)
{
    int count, len;
    char print_delim = ':';
    char addr_buf[ADDR_BUF_MAX];

    strncpy (addr_buf, mac_addr, ADDR_BUF_MAX);
    len = strlen (addr_buf);

    for (count = 0; count <= len; count += 2) {
        if (count >= len - 2)
            print_delim = ' ';

        if (addr_buf[count] != ':' && addr_buf[count + 1] != ':') {
            nk_printf ("%c%c%c", addr_buf[count], addr_buf[count + 1], print_delim);
            count++;
        }

        else if (addr_buf[count] != ':' && addr_buf[count + 1] == ':')
            nk_printf ("0%c%c", addr_buf[count], print_delim);
    }

    mac_addr = "";
    return 0;
}
