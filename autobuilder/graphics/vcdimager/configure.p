--- configure.orig	2008-01-18 12:04:03.000000000 -0800
+++ configure	2008-01-18 12:58:04.000000000 -0800
@@ -24319,7 +24319,7 @@
 rm -f core conftest.err conftest.$ac_objext conftest.$ac_ext
         if test "$cross_compiling" = yes; then
   case "$host" in
-     *-*-mingw32* | *-*-cygwin*)
+     *-*-mingw32* | *-*-cygwin* | *riscos*)
        bf_lsbf=1
      ;;
      *)
