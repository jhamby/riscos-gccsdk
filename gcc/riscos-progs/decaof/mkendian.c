/*
 * Determine byte-sex of local host to enable RISC OS (little endian) files
 * to be read/written.
 *
 * Andy Duplain, BT Customer Systems, Brighton, UK.  duplain@btcs.bt.co.uk
 */

#include <stdio.h>

void
main()
{
	register i;
	union {
		unsigned long l;
		unsigned short w;
		char c[4];
	} x;

	if (sizeof(x.l) != 4 || sizeof(x.w) != 2) {
		fprintf(stderr, "size mismatch in union... aborting\n");
		exit(1);
	}

	puts("/* endian.h:  local host specific byte-sex defines */");
	puts("#ifndef __ENDIAN_H");
	puts("#define __ENDIAN_H");
	x.l = 0x00010203;
	if (x.c[0] == 3 && x.c[1] == 2 && x.c[2] == 1 && x.c[3] == 0)
		puts("#define LITTLE_ENDIAN");
	else
		puts("#undef LITTLE_ENDIAN");
	for (i = 0; i < 4; i++)
		printf("#define WORD%d\t%d\n", i, 3 - x.c[i]);

	x.w = 0x0001;

	for (i = 0; i < 2; i++)
		printf("#define HALFWORD%d\t%d\n", i, 1 - x.c[i]);

	puts("#endif /* __ENDIAN_H */");
	exit(0);
}
