/**/
#include <stdio.h>

extern unsigned long long int fixunsdfdi (double b);
extern long long int fixdfdi (double b);
extern unsigned long long int fixunssfdi (float b);
extern long long int fixsfdi (float b);
extern float _fltsisf (unsigned long a);
extern double _fltdidf (unsigned long long a);

union v
{
  unsigned long long lval;
  unsigned int ival[2];
};

union w
{
  long long lval;
  unsigned int ival[2];
};


void uconv (float a)
{
  union v val;

  val.lval = fixunssfdi (a);
  printf ("64bit: %f (%llu) [0] = %08x, [1] = %08x\n",
  	  a, val.lval, val.ival[0], val.ival[1]);
}

void uconv1 (double a)
{
  union v val;

  val.lval = fixunsdfdi (a);
  printf ("64bit: %f (%llu) [0] = %08x, [1] = %08x\n",
  	  a, val.lval, val.ival[0], val.ival[1]);
}

void conv (float a)
{
  union w val;

  val.lval = fixsfdi (a);
  printf ("64bit: %f (%ll) [0] = %08x, [1] = %08x\n",
  	  a, val.lval, val.ival[0], val.ival[1]);
}

void conv1 (double a)
{
  union w val;

  val.lval = fixdfdi (a);
  printf ("64bit: %f (%ll) [0] = %08x, [1] = %08x\n",
  	  a, val.lval, val.ival[0], val.ival[1]);
}

void flt_conv (unsigned long a)
{
  printf ("flt_conv: %d (%f)\n", a, _fltsisf (a));
}

void flt_conv1 (unsigned long long a)
{
  double x = _fltdidf (a);

  printf ("flt_conv1: %.15f (%llu)\n", x, a);
}

int main (void)
{
  float a = 0.0;
  float b = 4.5;
  float c = 7.494;

  flt_conv1 (23952358259259376);

  flt_conv1 (1);
  flt_conv1 (0);
  flt_conv1 (1248);
  flt_conv1 (48);
  flt_conv1 (23409824);
  flt_conv1 (38553);
  flt_conv1 (239573255);
  flt_conv1 (485);

  flt_conv (1);
  flt_conv (0);
  flt_conv (1248);
  flt_conv (48);
  flt_conv (23409824);
  flt_conv (38553);
  flt_conv (239573255);
  flt_conv (485);

  conv1 ((double) 23952358259259376);
  conv ((float) 1.0);
  conv ((float) 1248.0);
  conv ((float) 48.0);
  conv ((float) 23409824.0);
  conv ((float) 38553.0);
  conv ((float) 239573255.0);


  conv1 (-16.0);
  conv1 (-16.527);
  conv1 (-16.4);
  conv1 (-4.7);
  conv1 (-129.0);
  conv1 (-127.0);
  conv1 (-0.000000045945);
  conv1 (-23492385.0);
  conv1 (-21349823596346.0);
  conv1 (-2346346359817259188224.0);

  uconv (a);
  uconv (b);
  uconv (c);
  uconv (2348234124.215);
  uconv (8235619493.2357);
  uconv (2348724987634003.340593268);
  uconv (23487249876340034523.340593268);

  uconv1 (a);
  uconv1 (b);
  uconv1 (c);
  uconv1 (2348234124.215);
  uconv1 (8235619493.2357);
  uconv1 (2348724987634003.340593268);
  uconv1 (23487249876340034523.340593268);


  return 0;
}
