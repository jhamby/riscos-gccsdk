/* rm
   Mimic "rm" for use in makefiles
   R.R.T.   v0.3 30/4/99 */

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#define PROGNAME "rm"

int main(int argc, char *argv[])
{
    int i, force= 0;

    for (i= 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0) force= 1;
        else
            if ((force && chmod(argv[i], S_IWRITE)) || remove(argv[i])) {
                perror(PROGNAME);
                exit(EXIT_FAILURE);
            }
    }

    exit(EXIT_SUCCESS);
}
