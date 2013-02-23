#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <linux/ip.h>
#include "nark.h"
#include "protocols.h"
#include "error.h"

void
print_payload (char *payload, int payload_len, struct iphdr *ip, unsigned int port)
{
    int i;
    int op_payload = get_payload ();

    if (payload_len > 0) {

        if (op_payload) {
            for (i = 0; i <= payload_len; i++) {
                if (isprint (payload[i]))
                    nk_printf ("%c", payload[i]);
                else
                    nk_printf ("0x%x ", (char)payload[i]);
            }

            nk_printf("\n\n");
        }
    }

    return;
}
