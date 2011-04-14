Index: libstdc++-v3/configure.ac
===================================================================
--- libstdc++-v3/configure.ac	(revision 167517)
+++ libstdc++-v3/configure.ac	(working copy)
@@ -92,6 +92,11 @@
 # up critical shell variables.
 GLIBCXX_CONFIGURE
 
+# Don't build a shared library for SCL:
+if [[ -n "`echo $CC | grep -- -mlibscl`" ]]; then
+  enable_shared=no
+fi
+
 if test "x${with_newlib}" != "xyes"; then
   AC_LIBTOOL_DLOPEN
 fi
@@ -111,7 +116,14 @@
 # Enable compiler support that doesn't require linking.
 GLIBCXX_ENABLE_SJLJ_EXCEPTIONS
 GLIBCXX_ENABLE_PCH($is_hosted)
+if [[ -n "`echo $CC | grep -- -mlibscl`" ]]; then
+  # SCL does not have thread support:
+  glibcxx_thread_h=gthr-single.h
+  gthread_file=${toplevel_srcdir}/gcc/${glibcxx_thread_h}
+  enable_thread=no
+else
 GLIBCXX_ENABLE_THREADS
+fi
 GLIBCXX_ENABLE_ATOMIC_BUILTINS
 GLIBCXX_ENABLE_DECIMAL_FLOAT
 
