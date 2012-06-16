Index: libstdc++-v3/configure.ac
===================================================================
--- libstdc++-v3/configure.ac	(revision 188171)
+++ libstdc++-v3/configure.ac	(working copy)
@@ -88,6 +88,11 @@
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
@@ -107,7 +112,14 @@
 # Enable compiler support that doesn't require linking.
 GLIBCXX_ENABLE_SJLJ_EXCEPTIONS
 GLIBCXX_ENABLE_PCH($is_hosted)
+if [[ -n "`echo $CC 2>&1 | grep -- -mlibscl`" ]]; then
+  # SCL does not have thread support:
+  glibcxx_thread_h=gthr-single.h
+  gthread_file=${toplevel_srcdir}/gcc/${glibcxx_thread_h}
+  enable_thread=no
+else
 GLIBCXX_ENABLE_THREADS
+fi
 GLIBCXX_ENABLE_ATOMIC_BUILTINS
 GLIBCXX_ENABLE_DECIMAL_FLOAT
 GLIBCXX_ENABLE_INT128_FLOAT128
@@ -336,7 +348,10 @@
 GLIBCXX_CONFIGURE_TESTSUITE
 
 # For gthread support.  Depends on GLIBCXX_ENABLE_SYMVERS.
-GLIBCXX_CHECK_GTHREADS
+# SCL does not have thread support:
+if [[ ! -n "`echo $CC 2>&1 | grep -- -mlibscl`" ]]; then
+  GLIBCXX_CHECK_GTHREADS
+fi
 
 # Define documentation rules conditionally.
 
@@ -421,11 +436,11 @@
 GLIBCXX_EXPORT_INCLUDES
 GLIBCXX_EXPORT_FLAGS
 
-if test "$enable_shared" = yes; then
-  PIC_CXXFLAGS="-prefer-pic"
-else
+#if test "$enable_shared" = yes; then
+#  PIC_CXXFLAGS="-prefer-pic"
+#else
   PIC_CXXFLAGS=
-fi
+#fi
 AC_SUBST(PIC_CXXFLAGS)
 
 dnl In autoconf 2.5x, AC_OUTPUT is replaced by four AC_CONFIG_* macros,
