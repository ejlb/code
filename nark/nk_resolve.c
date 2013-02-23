#include <string.h>
#include <netdb.h>
#include <resolv.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>


char *
nk_resolve (char *ip)
{
    // Funcion to resolve IPs to hostnames
    struct hostent *h_info;
    struct in_addr addr;

    inet_aton (ip, &addr);

    if ((h_info = gethostbyaddr (&addr, sizeof (addr), AF_INET)) == NULL)
        return ip;
    else
        return h_info->h_name;
}
