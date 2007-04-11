/*
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include <math.h>
#include <ctype.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/syslog.h>
#include <sys/stat.h>
#include <sys/select.h>

/*
 * This file exist merely to check that we are correctly linking. It does not
 * run.
 *
 */
static int dummy(void);

int main(int argc, const char * argv[])
{
  FILE * a;
  int b;
  char c[1024];
  char * d;
  double e;

  if(dummy)
    return EXIT_SUCCESS;

  a = fopen("/tmp/a", "r");
  fclose(a);

  b = rand();
  b = (int)floor(2.0);
  b = (int)cos(1.0);

  sprintf(c, "This is a test %f", 1.0);
  sscanf(c, "%d", &e);

  d = malloc(1024);
  free(d);
  strcpy(c, "This is text");
  memcpy(c, "123457890", 10);
  memset(c, 10, 10);
  if(isascii(*c))
  {
    puts("ASCII");
  }
  if(stricmp("aa", "AA") == 0)
  {
    puts ("AAAA");
  }
  if(argv[argc] == 0)
  {
    puts("aaa");
  }
  socket(0, 0, 0);
  bind(0, NULL, 0);
  listen(0,0);
  send(0, 0, 0, 0);
  wait(NULL);

/*
  openlog(NULL, 0, 0);
  closelog();
  syslog(0, "aaaa");
*/
  chmod("   ", 0);
  fchmod(0, 0);
  mkdir("aaa", 0);

  select(0, 0, 0, 0, 0);
  return EXIT_FAILURE;
}

static int dummy(void)
{
  return 1;
}
