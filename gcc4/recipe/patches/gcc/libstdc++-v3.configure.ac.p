--- libstdc++-v3/configure.ac.orig	2010-04-18 03:10:38.815386661 +0200
+++ libstdc++-v3/configure.ac	2010-04-18 03:10:12.635384269 +0200
@@ -82,6 +82,11 @@ AH_TEMPLATE(VERSION, [Version number of 
 # up critical shell variables.
 GLIBCXX_CONFIGURE
 
+# Don't build a shared library for SCL:
+if [[ -n "`echo $CC | grep -- -mlibscl`" ]]; then
+  enable_shared=no
+fi
+
 AC_LIBTOOL_DLOPEN
 AM_PROG_LIBTOOL
 AC_SUBST(enable_shared)
