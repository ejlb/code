#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <linux/tcp.h>
#include <linux/icmp.h>
#include <net/ethernet.h>
#include <netinet/ether.h>
#include <pthread.h>
#include <pcap.h>
#include "nark.h"

// Global packet headers for printing and manipulation
// FIXME
struct ether_header *eptr;
struct udphdr *udp;
struct iphdr *ip;
struct tcphdr *tcp;
struct icmphdr *icmp;
struct arphd *arp;
char *payload;


void *
nk_process_packet (u_char * arg, const struct pcap_pkthdr *pkthdr, u_char * packet)
{
    // Print the headers from packets recived by pcap_loop()
    int protocol;
    char *buf;

    eptr = (struct ether_header *) packet;

    payload = NULL;

    // If the packet is an arp packet dont parse an IP header
    if (ntohs (eptr->ether_type) == ETHERTYPE_ARP || ntohs (eptr->ether_type) == ETHERTYPE_REVARP) {
        buf = (packet + ETH_LEN);
        protocol = IPPROTO_ARP;
    } else {
        ip = (struct iphdr *) (packet + ETH_LEN);
        buf = (packet + ETH_LEN + IP_LEN);
        protocol = ip->protocol;
    }

    switch (protocol) {
    case IPPROTO_TCP:
        {
            // get the transport header
            tcp = (struct tcphdr *) buf;
            // get the payload
            payload = (buf + TCP_LEN);
            // print out the tcp packets headers
            print_tcp ();
            break;
        }

    case IPPROTO_UDP:
        {
            udp = (struct udphdr *) buf;
            payload = (buf + UDP_LEN);
            print_udp ();
            break;
        }

    case IPPROTO_ICMP:
        {
            icmp = (struct icmphdr *) buf;
            payload = (buf + ICMP_LEN);
            print_icmp ();
            break;
        }
    case IPPROTO_ARP:
        {
            arp = (struct arphd *) buf;
            payload = (buf + ARP_LEN);
            print_arp ();
            break;
        }
    }


    return 0;
}
