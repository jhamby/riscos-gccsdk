/* 1. Erase archive symbol table's timestamp from ar archives,
 * make it easy to `diff'.  (option -e)
 * 2. Check the sanity of timestamp. (option -c)
 *
 * $Id: teraser.c 2142 2011-11-10 15:29:59Z jkoshy $
 */

#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define TSPOS	24		/* position of timestamp */
#define TSLEN	10		/* length of timstamp string */
#define TDELAY	3		/* max delay allowed */
#define COUNTER	"/tmp/bsdar-test-total"
#define PASSED	"/tmp/bsdar-test-passed"

static void	usage(void);

int
main(int argc, char **argv)
{
	char opt;
	char checktime;
	char erasetime;
	char buf[TSLEN + 1], *_buf;
	char *tc;
	int fd;
	int ts;
	int now;
	FILE *ct, *ps;


	checktime = 0;
	erasetime = 0;
	tc = NULL;
	while ((opt = getopt(argc, argv, "cet:")) != -1) {
		switch(opt) {
		case 'c':
			checktime = 1;
			break;
		case 'e':
			erasetime = 1;
			break;
		case 't':
			tc = optarg;
			break;
		default:
			usage();
		}
	}

	argv += optind;
	if (*argv == NULL)
		usage();

	for (; *argv != NULL; argv++) {
		if (checktime) {
			if ((fd = open(*argv, O_RDONLY)) == -1) {
				fprintf(stderr,
				    "open %s failed(%s), skipping time check...\n,",
				    *argv, strerror(errno));
				goto ctend;
			}
			if ((lseek(fd, TSPOS, SEEK_SET)) == -1) {
				fprintf(stderr,
				    "lseek %s failed(%s), skipping...\n,",
				    *argv, strerror(errno));
				goto ctend;
			}
			if ((read(fd, buf, TSLEN)) != TSLEN) {
				fprintf(stderr,
				    "read %s failed(%s), skipping...\n,",
				    *argv, strerror(errno));
				goto ctend;
			}
			buf[TSLEN] = '\0';
			ts = atoi(buf);
			now = time(NULL);
			if (ts <= now && ts >= now - TDELAY) {
				fprintf(stderr, "%s - timestamp ok\n", tc);
				if ((ps = fopen(PASSED, "r")) != NULL) {
					_buf = fgets(buf, TSLEN, ps);
					snprintf(buf, TSLEN, "%d\n",
					    atoi(buf) + 1);
					fclose(ps);
				}
				if ((ps = fopen(PASSED, "w")) != NULL) {
					fputs(buf, ps);
					fclose(ps);
				}
			} else {
				fprintf(stderr, "%s - timestamp not ok\n", tc);
			}
			if ((ct = fopen(COUNTER, "r")) != NULL) {
				_buf = fgets(buf, TSLEN, ct);
				snprintf(buf, TSLEN, "%d\n", atoi(buf) + 1);
				fclose(ct);
			}
			if ((ct = fopen(COUNTER, "w")) != NULL) {
				fputs(buf, ct);
				fclose(ct);
			}

		ctend:
			close(fd);
		}

		if (erasetime) {
			if ((fd = open(*argv, O_RDWR)) == -1) {
				fprintf(stderr,
				    "open %s failed(%s), skipping time check...\n,",
				    *argv, strerror(errno));
				goto etend;
			}
			if ((lseek(fd, TSPOS, SEEK_SET)) == -1) {
				fprintf(stderr, "lseek %s failed(%s), skipping...,",
					*argv, strerror(errno));
				goto etend;
			}
			memset(buf, 32, TSLEN);
			if ((write(fd, buf, TSLEN)) != TSLEN)
				fprintf(stderr,
				    "read %s failed(%s), skipping...\n,",
				    *argv, strerror(errno));

		etend:
			close(fd);
		}
	}

	exit(EXIT_SUCCESS);
}

static void
usage(void)
{
	fprintf(stderr, "usage: teraser [-ce] [-t name] archive ...\n");
	exit(EXIT_FAILURE);
}
