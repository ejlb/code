#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

int
nk_console_init (void)
{

    char buf[512];
    DIR *dir;
    struct dirent *entry;

    snprintf (buf, 512, "%s/%s", INSTALL_LIB, CNSL_PLG);
    if ((dir = opendir (buf)) == NULL) {
        if ((dir = opendir (CNSL_PLG)) == NULL) {
            printf ("ERROR: Nark can't find your console_plugins dir its set as ./%s  or %s/%s\n",
                    CNSL_PLG, INSTALL_LIB, CNSL_PLG);
            printf ("FATAL: Exiting packet console...\n");
            exit (1);
        } else {
            printf ("[*] Local Modules\n");
            while ((entry = readdir (dir)) != NULL)
                printf ("[*] %s\n", entry->d_name);
            printf ("\n");
            closedir (dir);
        }
    } else {
        printf ("[*] Local Modules\n");
        while ((entry = readdir (dir)) != NULL)
            printf ("[*] %s\n", entry->d_name);
        printf ("\n");
        closedir (dir);
    }

    return 0;
}

void
nk_console (void)
{

    if ((nk_console_init ()) != 0) {
        printf ("ERROR: Nark can't find any plugins in ./%s or in %s/%s\n", CNSL_PLG, INSTALL_LIB,
                CNSL_PLG);
        return;
    }

    /* save files in linked list 
       - path
       - libname
       - version
       - description
       - usage
       - sid
       - function pointer
       - how many arguments
     */
    printf ("\n Nark> ");
    getchar ();
}
