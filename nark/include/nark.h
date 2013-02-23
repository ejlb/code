#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/icmp.h>
#include <net/ethernet.h>
#include <ncurses.h>
#include <getopt.h>
#include <pcap.h>

#define VERSION "1.00"
#define AUTHOR "ejlbell@gmail.com"

#define TCP_NAME "tcp"
#define UDP_NAME "udp"

#define IPPROTO_ARP -1

#ifndef PRINT_WIDTH
#define PRINT_WIDTH 60
#endif

// Packet headers sizes
#define UDP_LEN sizeof(struct udphdr)
#define TCP_LEN sizeof(struct tcphdr)
#define IP_LEN sizeof(struct iphdr)
#define ICMP_LEN sizeof(struct icmphdr)
#define ETH_LEN sizeof(struct ether_header)
#define ARP_LEN sizeof(struct arphd)


// console output formatting functions
int nk_printf (char *, ...);
void nk_printf_init (void);

void nk_console(void);

// protocol parsing and display functions
int print_mac (char *);
void print_packet (int);
void print_tcp (void);
void print_udp (void);
void print_icmp (void);
void print_arp (void);
void print_payload(char *,int,struct iphdr *, unsigned int);

// functions used by print_* to parse packet headers
char *convert_icmp (int, int);
char *convert_arp (int);
char *convert_type (int);
void *convert_port (unsigned int port, int flag, char *);

// Get and set functions
void set_port (int);
void set_host (int);
void set_payload (int);
void set_verbose (int);
void set_plog (int);
void set_pass_plugin (int);
void set_http_pass_plugin (int);
int get_port ();
int get_host ();
int get_payload ();
int get_plog ();
int get_verbose ();
int get_pass_plugin ();
int get_http_pass_plugin ();
void getset_init(void);

// Misc functions
char *nk_resolve (char *);
void usage (char *);
void *nk_process_packet (u_char *, const struct pcap_pkthdr *, u_char *);
int check_components(void);

// main plugin wrapper
void *pass_plugin_init(int, int);
char *pass_plugin(char *, int,void *);

// Arp header struct
struct arphd
 {
  unsigned short int ar_hrd;          /* Format of hardware address.  */
  unsigned short int ar_pro;          /* Format of protocol address.  */
  unsigned char ar_hln;               /* Length of hardware address.  */
  unsigned char ar_pln;               /* Length of protocol address.  */
  unsigned short int ar_op;           /* ARP opcode (command).  */

  unsigned char __ar_sha[ETH_ALEN];   /* Sender hardware address.  */
  unsigned char __ar_sip[4];          /* Sender IP address.  */
  unsigned char __ar_tha[ETH_ALEN];   /* Target hardware address.  */
  unsigned char __ar_tip[4];          /* Target IP address.  */
 };
						   
