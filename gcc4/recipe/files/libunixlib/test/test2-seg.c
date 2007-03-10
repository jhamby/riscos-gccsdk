
/**
 * UnixLib test 2
 *
 * Segmentation fault.
 *
 * Checks backtrace handler
 *
 */

#include <stdio.h>

int main (int argc, char *argv[]) {

  puts("Generating segmentation fault:");

  ((int *)0)[0] = 0;

  return 0;
}
