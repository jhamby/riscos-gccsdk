#include "swis.h"
#include <stdio.h>

#define Timer_Value 0x0490c2

double get_timerValue(void) {

  double timeSecs;
  int tvs;
  int tvm;

  tvs = 0;
  tvm = 0;

       _swix(Timer_Value, _OUTR(0,1), &tvs, &tvm);
        timeSecs = tvs + (tvm/1000000.0);

  return timeSecs;

}
