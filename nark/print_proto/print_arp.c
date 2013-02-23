#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <netinet/ether.h>
#include <linux/if.h>
#include <linux/if_ether.h>
#include <linux/icmp.h>
#include "nark.h"


void
print_arp ()
{
    struct in_addr d_addr, s_addr;
    extern struct arphd *arp;
    extern struct ether_header *eptr;
    char *daddr, *saddr;

    time_t current;

    int type, i;

    struct timeval tv;
    struct tm *t;
    struct ether_addr smac_addr;

    int op_host = get_host ();

    current = time (NULL);
    gettimeofday (&tv, NULL);
    t = gmtime (&current);
    type = ntohs (arp->ar_op);

    nk_printf ("%d:%d:%d-%ld  ", t->tm_hour, t->tm_min, t->tm_sec, tv.tv_usec);

    memcpy (&d_addr.s_addr, arp->__ar_tip, sizeof (arp->__ar_tip));
    daddr = inet_ntoa (d_addr);

    if (type == ARPOP_REQUEST) {

        if (op_host) {
            nk_printf ("%s who-has %s ", convert_arp (ntohs (arp->ar_op)), nk_resolve (daddr));

            memcpy (&s_addr.s_addr, arp->__ar_sip, sizeof (arp->__ar_sip));
            saddr = inet_ntoa (s_addr);
            nk_printf ("tell %s", nk_resolve (saddr));
        } else {
            nk_printf ("%s who-has %s ", convert_arp (ntohs (arp->ar_op)), daddr);

            memcpy (&s_addr.s_addr, arp->__ar_sip, sizeof (arp->__ar_sip));
            saddr = inet_ntoa (s_addr);
            nk_printf ("tell %s", saddr);
        }
    }

    else if (type == ARPOP_REPLY) {
        for (i = 0; i <= ETH_ALEN; i++)
            smac_addr.ether_addr_octet[i] = eptr->ether_shost[i];
        memcpy (&s_addr.s_addr, arp->__ar_sip, sizeof (arp->__ar_sip));
        saddr = inet_ntoa (s_addr);

        nk_printf ("%s %s is-at ", convert_arp (ntohs (arp->ar_op)), saddr);
        saddr = ether_ntoa (&smac_addr);

        print_mac (saddr);
    }

    nk_printf ("\n\n");
    return;
}
