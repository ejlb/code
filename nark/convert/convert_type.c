#include <net/ethernet.h>

 // Convert ethernet protocol code to text

char *
convert_type (int type)
{
    switch (type) {
    case ETHERTYPE_PUP:
        return "PUP";
    case ETHERTYPE_IP:
        return "IP";
    case ETHERTYPE_ARP:
        return "ARP";
    case ETHERTYPE_REVARP:
        return "RARP";
    }
    return "";

}
