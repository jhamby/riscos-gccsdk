--- configure.old	2002-12-17 14:43:18.000000000 +0000
+++ configure	2002-12-17 14:43:00.000000000 +0000
@@ -1564,7 +1564,7 @@
 
 echo checking handling of floating point exceptions
 rm -f fpe_check
-$CC $CFLAGS -DRETSIGTYPE=$ac_cv_type_signal -o fpe_check fpe_check.c $MATHLIB
+cc $CFLAGS -DRETSIGTYPE=$ac_cv_type_signal -o fpe_check fpe_check.c $MATHLIB
 if test -f fpe_check  ; then
    ./fpe_check 2>/dev/null
    status=$?
