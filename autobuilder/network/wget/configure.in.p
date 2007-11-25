--- configure.in.orig	2007-11-24 18:58:25.000000000 -0800
+++ configure.in	2007-11-24 19:08:14.000000000 -0800
@@ -141,6 +141,7 @@
 dnl
 case "$host_os" in
   *win32) exeext='.exe';;
+  riscos) exeext='$(AB_EXEEXT)';;
   *) exeext='';;
 esac
 AC_SUBST(exeext)
