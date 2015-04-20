--- Source/autotools/FindDependencies.m4.orig	2014-05-15 18:12:37.000000000 +0100
+++ Source/autotools/FindDependencies.m4	2014-06-15 18:26:31.511895362 +0100
@@ -117,6 +117,10 @@
         UNICODE_LIBS="-licui18n -licuuc"
         AC_CHECK_HEADERS([unicode/uchar.h], [], [AC_MSG_ERROR([Could not find ICU headers.])])
         ;;
+    *-*-riscos*)
+       UNICODE_CFLAGS="-I$GCCSDK_INSTALL_ENV/include"
+       UNICODE_LIBS="-L$GCCSDK_INSTALL_ENV/lib -licui18n -licuuc"
+       ;;
     *)
         AC_PATH_PROG(icu_config, icu-config, no)
         if test "$icu_config" = "no"; then
