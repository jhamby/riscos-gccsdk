--- libstdc++-v3/include/tr1/cmath.orig	2020-07-23 07:35:19.280405881 +0100
+++ libstdc++-v3/include/tr1/cmath	2020-07-24 17:16:30.801914818 +0100
@@ -164,139 +164,206 @@
 
   using ::asinh;
   using ::asinhf;
+#ifndef __NO_LONG_DOUBLE_MATH
   using ::asinhl;
+#endif
 
   using ::atanh;
   using ::atanhf;
+#ifndef __NO_LONG_DOUBLE_MATH
   using ::atanhl;
+#endif
 
   using ::cbrt;
   using ::cbrtf;
+#ifndef __NO_LONG_DOUBLE_MATH
   using ::cbrtl;
+#endif
 
   using ::copysign;
   using ::copysignf;
+#ifndef __NO_LONG_DOUBLE_MATH
   using ::copysignl;
+#endif
 
   using ::erf;
   using ::erff;
+#ifndef __NO_LONG_DOUBLE_MATH
   using ::erfl;
+#endif
 
   using ::erfc;
   using ::erfcf;
+#ifndef __NO_LONG_DOUBLE_MATH
   using ::erfcl;
+#endif
 
   using ::exp2;
   using ::exp2f;
+#ifndef __NO_LONG_DOUBLE_MATH
   using ::exp2l;
+#endif
 
   using ::expm1;
   using ::expm1f;
+#ifndef __NO_LONG_DOUBLE_MATH
   using ::expm1l;
+#endif
 
   using ::fdim;
   using ::fdimf;
+#ifndef __NO_LONG_DOUBLE_MATH
   using ::fdiml;
+#endif
 
   using ::fma;
   using ::fmaf;
+#ifndef __NO_LONG_DOUBLE_MATH
   using ::fmal;
+#endif
 
   using ::fmax;
   using ::fmaxf;
+#ifndef __NO_LONG_DOUBLE_MATH
   using ::fmaxl;
+#endif
 
   using ::fmin;
   using ::fminf;
+#ifndef __NO_LONG_DOUBLE_MATH
   using ::fminl;
+#endif
 
   using ::hypot;
   using ::hypotf;
+#ifndef __NO_LONG_DOUBLE_MATH
   using ::hypotl;
+#endif
 
   using ::ilogb;
   using ::ilogbf;
+#ifndef __NO_LONG_DOUBLE_MATH
   using ::ilogbl;
 
   using ::lgamma;
   using ::lgammaf;
+#ifndef __NO_LONG_DOUBLE_MATH
   using ::lgammal;
+#endif
 
   using ::llrint;
   using ::llrintf;
+#ifndef __NO_LONG_DOUBLE_MATH
   using ::llrintl;
+#endif
 
   using ::llround;
   using ::llroundf;
+#ifndef __NO_LONG_DOUBLE_MATH
   using ::llroundl;
+#endif
 
   using ::log1p;
   using ::log1pf;
+#ifndef __NO_LONG_DOUBLE_MATH
   using ::log1pl;
+#endif
 
   using ::log2;
   using ::log2f;
+#ifndef __NO_LONG_DOUBLE_MATH
   using ::log2l;
+#endif
 
   using ::logb;
   using ::logbf;
+#ifndef __NO_LONG_DOUBLE_MATH
   using ::logbl;
+#endif
 
   using ::lrint;
   using ::lrintf;
+#ifndef __NO_LONG_DOUBLE_MATH
   using ::lrintl;
+#endif
 
   using ::lround;
   using ::lroundf;
+#ifndef __NO_LONG_DOUBLE_MATH
   using ::lroundl;
+#endif
 
   using ::nan;
   using ::nanf;
+#ifndef __NO_LONG_DOUBLE_MATH
   using ::nanl;
+#endif
 
   using ::nearbyint;
   using ::nearbyintf;
+#ifndef __NO_LONG_DOUBLE_MATH
   using ::nearbyintl;
+#endif
 
   using ::nextafter;
   using ::nextafterf;
+#ifndef __NO_LONG_DOUBLE_MATH
   using ::nextafterl;
+#endif
 
   using ::nexttoward;
   using ::nexttowardf;
+#ifndef __NO_LONG_DOUBLE_MATH
   using ::nexttowardl;
+#endif
 
   using ::remainder;
   using ::remainderf;
+#ifndef __NO_LONG_DOUBLE_MATH
   using ::remainderl;
+#endif
 
   using ::remquo;
   using ::remquof;
+#ifndef __NO_LONG_DOUBLE_MATH
   using ::remquol;
+#endif
 
   using ::rint;
   using ::rintf;
+#ifndef __NO_LONG_DOUBLE_MATH
   using ::rintl;
+#endif
 
   using ::round;
   using ::roundf;
+#ifndef __NO_LONG_DOUBLE_MATH
   using ::roundl;
+#endif
 
   using ::scalbln;
   using ::scalblnf;
+#ifndef __NO_LONG_DOUBLE_MATH
   using ::scalblnl;
+#endif
 
   using ::scalbn;
   using ::scalbnf;
+#ifndef __NO_LONG_DOUBLE_MATH
   using ::scalbnl;
+#endif
 
   using ::tgamma;
   using ::tgammaf;
+#ifndef __NO_LONG_DOUBLE_MATH
   using ::tgammal;
+#endif
 
   using ::trunc;
   using ::truncf;
+#ifndef __NO_LONG_DOUBLE_MATH
   using ::truncl;
+#endif
 
 #endif
 
