#include <stdio.h>

/* Get and set functions to pass arguments around the program 
 * 1 to set a feature and 0 to turn it off 
 */

static int port_arg;
static int host_arg;
static int payload_arg;
static int verbose_arg;
static int pass_plugin_arg;
static int http_pass_plugin_arg;
static int plog_arg;

// Print port-names if possible instead of port numbers

void
set_port (int arg)
{
    port_arg = arg;
}

int
get_port (void)
{
    return port_arg;
}

// Resolve hosts names of IP addresses

void
set_host (int arg)
{
    host_arg = arg;
}

int
get_host (void)
{
    return host_arg;
}

// Log all output to a xml file

void
set_plog (int arg)
{
    plog_arg = arg;
}

int
get_plog (void)
{
    return plog_arg;
}

// Print out the payload of packets

void
set_payload (int arg)
{
    payload_arg = arg;
}

int
get_payload (void)
{
    return payload_arg;
}

// Verbose packet output
// It automatically sets payload as well

void
set_verbose (int arg)
{
    verbose_arg = arg;
    set_payload (arg);
}

int
get_verbose (void)
{
    return verbose_arg;
}

// To activate password parsing modules

void
set_pass_plugin (int arg)
{
    pass_plugin_arg = arg;
    set_payload (arg);
}

int
get_pass_plugin (void)
{
    return pass_plugin_arg;
}

// To activate http application password parsing modules

void
set_http_pass_plugin (int arg)
{
    http_pass_plugin_arg = arg;
    set_payload (arg);
}

int
get_http_pass_plugin (void)
{
    return http_pass_plugin_arg;
}




// Initialise options to default arrangement

void
getset_init (void)
{

    set_port (1);
    set_verbose (0);
    set_host (0);
    set_plog (0);
    set_pass_plugin (0);
    set_http_pass_plugin (0);
}
