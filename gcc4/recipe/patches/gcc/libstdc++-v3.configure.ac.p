--- libstdc++-v3/configure.ac.orig	2010-05-12 01:59:36.178786562 +0200
+++ libstdc++-v3/configure.ac	2010-05-12 01:59:00.789010932 +0200
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
@@ -111,7 +116,14 @@ GLIBCXX_ENABLE_CXX_FLAGS
 GLIBCXX_ENABLE_FULLY_DYNAMIC_STRING([no])
 
 # No surprises, no surprises...
+if [[ -n "`echo $CC | grep -- -mlibscl`" ]]; then
+  # SCL does not have thread support:
+  glibcxx_thread_h=gthr-single.h
+  gthread_file=${toplevel_srcdir}/gcc/${glibcxx_thread_h}
+  enable_thread=no
+else
 GLIBCXX_ENABLE_THREADS
+fi
 if test $atomicity_dir = cpu/generic ; then
   AC_MSG_WARN([No native atomic operations are provided for this platform.])
   if test $target_thread_file = single; then
