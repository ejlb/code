#include "icmp.h"

// Convert an icmp code to a character representation of it;

char *
convert_icmp (int code, int type)
{
    switch (type) {
    case ICMP_ECHOREPLY:
        return "Echo-Reply";
    case ICMP_DEST_UNREACH:
        return "Destination-Unreachable";
    case ICMP_SOURCE_QUENCH:
        return "Source-Quench";
    case ICMP_REDIRECT:
        return "Redirect";
    case ICMP_ECHO:
        return "Echo";
    case ICMP_TIME_EXCEEDED:
        return "Time-Exceeded";
    case ICMP_PARAMETERPROB:
        return "Parameter-Problem";
    case ICMP_TIMESTAMP:
        return "Timestamp";
    case ICMP_TIMESTAMPREPLY:
        return "Timestamp-Reply";
    case ICMP_INFO_REQUEST:
        return "Info";
    case ICMP_INFO_REPLY:
        return "Info-Reply";
    case ICMP_ADDRESS:
        return "Address-Mask";
    case ICMP_ADDRESSREPLY:
        return "Address-Mask-Reply";
    }

    return "";
}
