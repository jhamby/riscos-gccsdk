/* > c.readline
 *
 * RISC OS readline
 *
 * Use system readline to allow cursor-editing and history
 * to be handled by system, taskwindow module of your favorite
 * editor or e.g. Olly Bett's LineEdit module.
 *
 * (c) B.A. Brand, 2001 04 14
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef __GNUC__
#include <sys/os.h>
#include <sys/swis.h>
#else
#define __UNIXLIB_INTERNALS
#include <oslib/os.h>
#undef __UNIXLIB_INTERNALS
#ifndef os_swi
#define os_swi(a,b) __os_swi(a,b)
#endif
#include <swis.h>
#endif


/* Some definitions from plot.h (UnixLib's and Acorn/Norcroft's os_error
   conflicts with Gnuplot's one */

char *gp_alloc (unsigned long size, char *message);
void int_error(char str[], int t_num);
#define NO_CARET (-1)


/* Use same value as GnuPlot's readline. */
#define MAXBUF 1024

char * readline (char * prompt) {
  char * buf;
  int reg[10];

  /* alloc memory for line, wil be free()d by caller of readline. */
  buf = (char*) gp_alloc(MAXBUF, "readline");

  fprintf(stderr,"%s",prompt);
  fflush(stderr);

  memset(reg, 0, sizeof(reg));
  reg[0] = (int) buf;
  reg[1] = MAXBUF;
  reg[2] = 32;
  reg[3] = 255;
  if (os_swi(OS_ReadLine, reg)) {
    fprintf(stderr,"\r"); fflush(stderr);
    free(buf);
    return NULL;
  }
  fprintf(stderr,"\r"); fflush(stderr);
  buf[reg[1]+1] = '\0'; /* make a ASCIIZ string from an ASCIIctrl string */
  return buf;
}

/* Dummy add_history(), we're using LineEdit or any other
   line editer module that might be present. */
void add_history(char * line) {}
