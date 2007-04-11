
/**
 * UnixLib test 3
 *
 * Error handler.
 *
 * Create a floating point exception - this invokes UnixLib's
 * error environment handler.
 *
 */

#include <stdio.h>
#include <math.h>

int main (int argc, char *argv[]) {
  float d;

  puts("Calling log(0)");

  d = log(0);

  return 0;
}
