#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <pcap.h>
#include "nark.h"
#include "error.h"

// Main program file for nark network analyser

typedef struct {
    char *dev;
    char *filter;
    int promisc;
    int console;
} opts_t;


int counter = 0;

int
main (int argc, char *argv[])
{
    char errbuf[PCAP_ERRBUF_SIZE];
    struct bpf_program filter;
    bpf_u_int32 net, mask;
    pcap_t *handle;             /* Session handle */
    pcap_dumper_t *log = NULL;  /* Log handle */

    opts_t opts;                // Option vars for features that dont use set/get functions

    int ch;
    int option_index = 0;

    static struct option long_options[] = {
        {"interface", 1, 0, 'i'},
        {"filter", 1, 0, 'f'},
        {"xml-log", 1, 0, 'L'},
        {"verbose", 0, 0, 'v'},
        {"help", 0, 0, 'h'},
        {"enable-resolve", 0, 0, 'r'},
        {"disable-portname", 0, 0, 'n'},
        {"disable-promisc", 0, 0, 'p'},
        {0, 0, 0, 0}
    };

    // Set all argument values to there default settings
    opts.dev = NULL;
    opts.filter = NULL;
    opts.promisc = true;

    getset_init ();

    while ((ch = getopt_long (argc, argv, "i:f:L:rpdnPhvIH", long_options, &option_index)) != EOF) {
        switch (ch) {

        case 'h':
            {
                usage (argv[0]);
                exit (0);
            }

        case 'f':
            {
                opts.filter = optarg;
                break;
            }

        case 'i':
            {
                opts.dev = optarg;
                break;
            }

        case 'n':
            {
                set_port (false);
                break;
            }

        case 'p':
            {
                opts.promisc = false;
                break;
            }
        case 'v':
            {
                set_verbose (true);
                break;
            }

        case 'r':
            {
                set_host (true);
                break;
            }

        case 'L':
            {
                set_plog (true);
                break;
            }

        case 'P':
            {
                set_pass_plugin (true);
                break;
            }

        case 'H':
            {
                set_http_pass_plugin (true);
                break;
            }

        case '?':
            {
                exit (0);
                break;
            }

        default:
            printf ("%s: unknown option\n", optarg);

        }
    }

    memset (errbuf, 0, PCAP_ERRBUF_SIZE);

    // Check sniffing device
    if (opts.dev == NULL) {
        if ((opts.dev = pcap_lookupdev (errbuf)) == NULL) {
            fprintf (stderr, "ERROR: Cannot find device to sniff on: %s\n", errbuf);
            exit (-1);
        }
    }
    // Open device to sniff on
    if ((handle = pcap_open_live (opts.dev, BUFSIZ, opts.promisc, -1, errbuf)) == NULL) {
        fprintf (stderr, "ERROR: Cannot open %s to sniff on: %s\n", opts.dev, errbuf);
        exit (-1);
    }
    // Lookup the network infomation the the device
    pcap_lookupnet (opts.dev, &net, &mask, errbuf);
    if ((strlen (errbuf)) > 1) {
        fprintf (stderr, "ERROR: Cannot lookup network infomation on %s : %s\n", opts.dev, errbuf);
        exit (-1);
    }

    printf ("\nNark sniffing (default mode) on %s. For help and options use '--help'\n", opts.dev);
    printf ("For the protocol manipulation console use '--pkt-console'\n");

    // Apply any filters the user specified
    if (opts.filter != NULL) {
        if ((pcap_compile (handle, &filter, opts.filter, 0, net)) == -1)
            printf ("pcap_compile: %s\n\n", pcap_geterr (handle));
        else {
            pcap_setfilter (handle, &filter);
            printf ("Using filter (%s)\n\n", opts.filter);
        }
    } else
        printf ("\n");

    // start reading the packets with nk_process_packet()
    pcap_loop (handle, -1, (void *) nk_process_packet, (void *) log);
    pcap_close (handle);

    return 0;
}
