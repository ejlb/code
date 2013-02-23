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
#include <linux/udp.h>
#include <linux/if.h>
#include "nark.h"

void
print_udp ()
{
    struct in_addr d_addr, s_addr;
    extern struct iphdr *ip;
    extern struct udphdr *udp;
    extern char *payload;
    extern struct ether_header *eptr;

    char *daddr = "";
    char *saddr = "";
    char *port_name;
    time_t current;

    u_short s_port = 0;
    u_short d_port = 0;

    int payload_len = -1;
    int i, type;

    int op_payload;
    int op_port;
    int op_host;

    struct ether_addr smac_addr;
    struct ether_addr dmac_addr;
    struct timeval tv;
    struct tm *t;

    d_addr.s_addr = ip->daddr;
    s_addr.s_addr = ip->saddr;

    op_payload = get_payload ();
    op_port = get_port ();
    op_host = get_host ();

/* copy mac address to [s,d]mac_addr */

    for (i = 0; i <= ETH_ALEN; i++) {
        smac_addr.ether_addr_octet[i] = eptr->ether_shost[i];
        dmac_addr.ether_addr_octet[i] = eptr->ether_dhost[i];
    }

/* assign some variables we need for the output */


    current = time (NULL);
    t = gmtime (&current);
    gettimeofday (&tv, NULL);
    type = ntohs (eptr->ether_type);

/* print time and ethernet structure */

    nk_printf ("%d:%d:%d-%ld  ", t->tm_hour, t->tm_min, t->tm_sec, tv.tv_usec);

    saddr = ether_ntoa (&smac_addr);
    print_mac (saddr);

    nk_printf ("-> ");
    daddr = ether_ntoa (&dmac_addr);

    print_mac (daddr);

    nk_printf (" Type:0x%X [%s]  Len:%d  ", type, convert_type (type), ntohs (ip->tot_len));

    s_port = ntohs (udp->source);
    d_port = ntohs (udp->dest);

    saddr = inet_ntoa (s_addr);

    if (op_host)
        nk_printf ("%s:", nk_resolve (saddr));
    else
        nk_printf ("%s:", saddr);

    port_name = convert_port (s_port, op_port, UDP_NAME);

    if (port_name != NULL)
        nk_printf ("%s", port_name);
    else
        nk_printf ("%d", s_port);

    nk_printf (" -> ");

    daddr = inet_ntoa (d_addr);

    if (op_host)
        nk_printf ("%s:", nk_resolve (daddr));
    else
        nk_printf ("%s:", daddr);
    port_name = convert_port (d_port, op_port, UDP_NAME);

    if (port_name != NULL)
        nk_printf ("%s ", port_name);
    else
        nk_printf ("%d ", d_port);

    nk_printf (" UDP  ");

    payload_len = (ntohs (ip->tot_len) - UDP_LEN - IP_LEN);

/* ip fields */

    nk_printf ("TOS:0x%x  ID:%d  TTL:%d\n\n", ip->tos, ntohs (ip->id), ip->ttl);

    print_payload (payload, payload_len, ip, d_port);
    return;
}
