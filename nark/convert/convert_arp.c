//#include <linux/if_arp.h>
// Convert ar_op from an arp header to a string

#define ARPOP_REQUEST   1               /* ARP request                  */
#define ARPOP_REPLY     2               /* ARP reply                    */
#define ARPOP_RREQUEST  3               /* RARP request                 */
#define ARPOP_RREPLY    4 

char *
convert_arp (int code)
{

    switch (code) {
	    case ARPOP_REQUEST:
	        return "ARP-Request";
	    case ARPOP_REPLY:
	        return "ARP-Reply";
	    case ARPOP_RREQUEST:
	        return "RARP-Request";
	    case ARPOP_RREPLY:
	        return "RARP-Reply";
    }
    return "ARP";
}
