/* touch
   Mimic "touch" for use in makefiles
   Based on code in Make
   R.R.T.   v0.2 30/4/99 */

#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <utime.h>

#define PROGNAME "touch"

int main(int argc, char *argv[])
{
    int i, err, ret;

    for (i= 1; i < argc; i++)
        if (ret= utime(argv[i], NULL)) {

	/* create the file if it did not exist */
	    if (errno == ENOENT) {
	    	ret = open(argv[i], O_CREAT | O_TRUNC, S_IWRITE);
	    	if (ret != -1) close(ret);
	    }

            if (ret == -1) {
            	perror(PROGNAME);
            	exit(EXIT_FAILURE);
            }
        }

    exit(EXIT_SUCCESS);
}
