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
#include <linux/ip.h>
#include <linux/if.h>
#include <linux/icmp.h>
#include "nark.h"


void
print_icmp ()
{
    struct in_addr d_addr, s_addr;
    extern struct iphdr *ip;
    extern struct icmphdr *icmp;
    extern char *payload;
    extern struct ether_header *eptr;

    char *daddr, *saddr;
    time_t current;

    int i, type;
    int payload_len;

    struct ether_addr smac_addr;
    struct ether_addr dmac_addr;
    struct timeval tv;
    struct tm *t;

    int op_host = get_host ();

    d_addr.s_addr = ip->daddr;
    s_addr.s_addr = ip->saddr;


    for (i = 0; i <= ETH_ALEN; i++) {
        smac_addr.ether_addr_octet[i] = eptr->ether_shost[i];
        dmac_addr.ether_addr_octet[i] = eptr->ether_dhost[i];
    }

    current = time (NULL);
    gettimeofday (&tv, NULL);

    t = gmtime (&current);
    type = ntohs (eptr->ether_type);

    nk_printf ("%d:%d:%d-%ld  ", t->tm_hour, t->tm_min, t->tm_sec, tv.tv_usec);


    daddr = ether_ntoa (&dmac_addr);
    print_mac (daddr);
    nk_printf ("-> ");
    saddr = ether_ntoa (&smac_addr);
    print_mac (saddr);

    nk_printf (" Type:0x%X [%s]  Len:%d  ", type, convert_type (type), ntohs (ip->tot_len));

    saddr = inet_ntoa (s_addr);

    if (op_host) {
        nk_printf ("%s -> ", nk_resolve (saddr));
        daddr = inet_ntoa (d_addr);
        nk_printf ("%s", nk_resolve (daddr));
    } else {
        nk_printf ("%s -> ", saddr);
        daddr = inet_ntoa (d_addr);
        nk_printf ("%s ", daddr);
    }

    nk_printf (" ICMP  %s  Type:%d  Code:%d  ",
               convert_icmp (icmp->code, icmp->type), icmp->type, icmp->code);

    if (icmp->type == 0 || icmp->type == 8) {
        nk_printf ("[ EchoID:%d  Seq:%d  ", ntohs ((icmp->un).echo.id),
                   ntohs ((icmp->un).echo.sequence));

        nk_printf ("TOS:0x%x  ID:%d  TTL:%d ]  ", ip->tos, ntohs (ip->id), ip->ttl);
    }


    /* ip */
    nk_printf ("TOS:0x%x  ID:%d  TTL:%d", ip->tos, ntohs (ip->id), ip->ttl);
    nk_printf ("\n\n");

    payload_len = (ntohs (ip->tot_len) - ICMP_LEN - IP_LEN);
    print_payload (payload, payload_len, ip, 0);
    return;
}
