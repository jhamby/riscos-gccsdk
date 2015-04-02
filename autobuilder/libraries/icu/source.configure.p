--- source/configure.orig	2014-02-07 14:30:29.000000000 +0000
+++ source/configure	2014-02-07 15:06:51.789875800 +0000
@@ -4942,6 +4942,7 @@
 *-dec-osf*) icu_cv_host_frag=mh-alpha-osf ;;
 *-*-nto*)	icu_cv_host_frag=mh-qnx ;;
 *-ncr-*)	icu_cv_host_frag=mh-mpras ;;
+*-*-riscos*)	icu_cv_host_frag=mh-linux ;;
 *) 		icu_cv_host_frag=mh-unknown ;;
 esac
 
@@ -6032,7 +6033,7 @@
 # Check to see if genccode can generate simple assembly.
 GENCCODE_ASSEMBLY=
 case "${host}" in
-*-linux*|*-kfreebsd*-gnu*|i*86-*-*bsd*|i*86-pc-gnu)
+*-linux*|*-kfreebsd*-gnu*|i*86-*-*bsd*|i*86-pc-gnu|*-*-riscos*)
     if test "$GCC" = yes; then
         # We're using gcc, and the simple -a gcc command line works for genccode
         GENCCODE_ASSEMBLY="-a gcc"
