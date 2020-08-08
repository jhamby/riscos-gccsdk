--- libstdc++-v3/acinclude.m4.orig	2020-07-23 07:35:19.164404603 +0100
+++ libstdc++-v3/acinclude.m4	2020-07-24 17:16:30.825914620 +0100
@@ -1859,111 +1859,181 @@
 		  typedef float_t   my_float_t;
 		  acosh(0.0);
 		  acoshf(0.0f);
+		  #ifndef __NO_LONG_DOUBLE_MATH
 		  acoshl(0.0l);
+		  #endif
 		  asinh(0.0);
 		  asinhf(0.0f);
+		  #ifndef __NO_LONG_DOUBLE_MATH
 		  asinhl(0.0l);
+		  #endif
 		  atanh(0.0);
 		  atanhf(0.0f);
+		  #ifndef __NO_LONG_DOUBLE_MATH
 		  atanhl(0.0l);
+		  #endif
 		  cbrt(0.0);
 		  cbrtf(0.0f);
+		  #ifndef __NO_LONG_DOUBLE_MATH
 		  cbrtl(0.0l);
+		  #endif
 		  copysign(0.0, 0.0);
 		  copysignf(0.0f, 0.0f);
+		  #ifndef __NO_LONG_DOUBLE_MATH
 		  copysignl(0.0l, 0.0l);
+		  #endif
 		  erf(0.0);
 		  erff(0.0f);
+		  #ifndef __NO_LONG_DOUBLE_MATH
 		  erfl(0.0l);
+		  #endif
 		  erfc(0.0);
 		  erfcf(0.0f);
+		  #ifndef __NO_LONG_DOUBLE_MATH
 		  erfcl(0.0l);
+		  #endif
 		  exp2(0.0);
 		  exp2f(0.0f);
+		  #ifndef __NO_LONG_DOUBLE_MATH
 		  exp2l(0.0l);
+		  #endif
 		  expm1(0.0);
 		  expm1f(0.0f);
+		  #ifndef __NO_LONG_DOUBLE_MATH
 		  expm1l(0.0l);
+		  #endif
 		  fdim(0.0, 0.0);
 		  fdimf(0.0f, 0.0f);
+		  #ifndef __NO_LONG_DOUBLE_MATH
 		  fdiml(0.0l, 0.0l);
+		  #endif
 		  fma(0.0, 0.0, 0.0);
 		  fmaf(0.0f, 0.0f, 0.0f);
+		  #ifndef __NO_LONG_DOUBLE_MATH
 		  fmal(0.0l, 0.0l, 0.0l);
+		  #endif
 		  fmax(0.0, 0.0);
 		  fmaxf(0.0f, 0.0f);
+		  #ifndef __NO_LONG_DOUBLE_MATH
 		  fmaxl(0.0l, 0.0l);
+		  #endif
 		  fmin(0.0, 0.0);
 		  fminf(0.0f, 0.0f);
+		  #ifndef __NO_LONG_DOUBLE_MATH
 		  fminl(0.0l, 0.0l);
+		  #endif
 		  hypot(0.0, 0.0);
 		  hypotf(0.0f, 0.0f);
+		  #ifndef __NO_LONG_DOUBLE_MATH
 		  hypotl(0.0l, 0.0l);
+		  #endif
 		  ilogb(0.0);
 		  ilogbf(0.0f);
+		  #ifndef __NO_LONG_DOUBLE_MATH
 		  ilogbl(0.0l);
+		  #endif
 		  lgamma(0.0);
 		  lgammaf(0.0f);
+		  #ifndef __NO_LONG_DOUBLE_MATH
 		  lgammal(0.0l);
+		  #endif
 		  #ifndef __APPLE__ /* see below */
 		  llrint(0.0);
 		  llrintf(0.0f);
+		  #ifndef __NO_LONG_DOUBLE_MATH
 		  llrintl(0.0l);
+		  #endif
 		  llround(0.0);
 		  llroundf(0.0f);
+		  #ifndef __NO_LONG_DOUBLE_MATH
 		  llroundl(0.0l);
 		  #endif
+		  #endif
 		  log1p(0.0);
 		  log1pf(0.0f);
+		  #ifndef __NO_LONG_DOUBLE_MATH
 		  log1pl(0.0l);
+		  #endif
 		  log2(0.0);
 		  log2f(0.0f);
+		  #ifndef __NO_LONG_DOUBLE_MATH
 		  log2l(0.0l);
+		  #endif
 		  logb(0.0);
 		  logbf(0.0f);
+		  #ifndef __NO_LONG_DOUBLE_MATH
 		  logbl(0.0l);
+		  #endif
 		  lrint(0.0);
 		  lrintf(0.0f);
+		  #ifndef __NO_LONG_DOUBLE_MATH
 		  lrintl(0.0l);
+		  #endif
 		  lround(0.0);
 		  lroundf(0.0f);
+		  #ifndef __NO_LONG_DOUBLE_MATH
 		  lroundl(0.0l);
+		  #endif
 		  nan(0);
 		  nanf(0);
+		  #ifndef __NO_LONG_DOUBLE_MATH
 		  nanl(0);
+		  #endif
 		  nearbyint(0.0);
 		  nearbyintf(0.0f);
+		  #ifndef __NO_LONG_DOUBLE_MATH
 		  nearbyintl(0.0l);
+		  #endif
 		  nextafter(0.0, 0.0);
 		  nextafterf(0.0f, 0.0f);
+		  #ifndef __NO_LONG_DOUBLE_MATH
 		  nextafterl(0.0l, 0.0l);
+		  #endif
 		  nexttoward(0.0, 0.0);
 		  nexttowardf(0.0f, 0.0f);
+		  #ifndef __NO_LONG_DOUBLE_MATH
 		  nexttowardl(0.0l, 0.0l);
+		  #endif
 		  remainder(0.0, 0.0);
 		  remainderf(0.0f, 0.0f);
+		  #ifndef __NO_LONG_DOUBLE_MATH
 		  remainderl(0.0l, 0.0l);
+		  #endif
 		  remquo(0.0, 0.0, 0);
 		  remquof(0.0f, 0.0f, 0);
+		  #ifndef __NO_LONG_DOUBLE_MATH
 		  remquol(0.0l, 0.0l, 0);
+		  #endif
 		  rint(0.0);
 		  rintf(0.0f);
+		  #ifndef __NO_LONG_DOUBLE_MATH
 		  rintl(0.0l);
+		  #endif
 		  round(0.0);
 		  roundf(0.0f);
+		  #ifndef __NO_LONG_DOUBLE_MATH
 		  roundl(0.0l);
+		  #endif
 		  scalbln(0.0, 0l);
 		  scalblnf(0.0f, 0l);
+		  #ifndef __NO_LONG_DOUBLE_MATH
 		  scalblnl(0.0l, 0l);
+		  #endif
 		  scalbn(0.0, 0);
 		  scalbnf(0.0f, 0);
+		  #ifndef __NO_LONG_DOUBLE_MATH
 		  scalbnl(0.0l, 0);
+		  #endif
 		  tgamma(0.0);
 		  tgammaf(0.0f);
+		  #ifndef __NO_LONG_DOUBLE_MATH
 		  tgammal(0.0l);
+		  #endif
 		  trunc(0.0);
 		  truncf(0.0f);
+		  #ifndef __NO_LONG_DOUBLE_MATH
 		  truncl(0.0l);
+		  #endif
 		 ],[glibcxx_cv_c99_math_tr1=yes], [glibcxx_cv_c99_math_tr1=no])
   ])
   AC_MSG_RESULT($glibcxx_cv_c99_math_tr1)
@@ -2349,7 +2419,7 @@
 
 m4_pushdef([n_syserr], [1])dnl
 m4_foreach([syserr], [EOWNERDEAD, ENOTRECOVERABLE, ENOLINK, EPROTO, ENODATA,
-		      ENOSR, ENOSTR, ETIME, EBADMSG, ECANCELED,
+		      ENOMSG, ENOSR, ENOSTR, ETIME, EBADMSG, ECANCELED,
 		      EOVERFLOW, ENOTSUP, EIDRM, ETXTBSY,
 		      ECHILD, ENOSPC, EPERM,
 		      ETIMEDOUT, EWOULDBLOCK],
