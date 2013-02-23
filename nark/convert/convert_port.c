#include <stdio.h>
#include <netdb.h>

struct servent *name;

// Convert port number to port name

char *
convert_port (unsigned int port, int flag, char *proto)
{
    if (flag) {
        if ((name = getservbyport (htons (port), proto)) != NULL) {
            return name->s_name;
        }
    }

    return NULL;


}
