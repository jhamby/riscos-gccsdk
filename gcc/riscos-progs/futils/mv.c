/* mv
   Mimic "mv" for use in makefiles
   R.R.T.   v0.1 25/2/97 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define PROGNAME "mv"

int main(int argc, char *argv[])
{
    if (argc != 3) {
    	fprintf(stderr, "Usage: " PROGNAME " from to\n");
    	exit(EXIT_FAILURE);
    }

    if (rename(argv[1], argv[2]) == -1) {
        perror(PROGNAME);
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
