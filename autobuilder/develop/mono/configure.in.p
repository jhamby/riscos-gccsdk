--- configure.in.orig	2013-07-30 02:50:24.000000000 +0100
+++ configure.in	2013-09-12 19:51:14.587500656 +0100
@@ -93,6 +93,7 @@
 target_win32=no
 platform_android=no
 platform_darwin=no
+platform_riscos=no
 case "$host" in
 	*-mingw*|*-*-cygwin*)
 		AC_DEFINE(HOST_WIN32,1,[Host Platform is Win32])
@@ -346,6 +347,16 @@
 		libgc_threads=pthreads
 		use_sigposix=yes
 		;;
+	*-*-riscos*)
+		host_win32=no
+		platform_riscos=yes
+		CPPFLAGS="$CPPFLAGS -DGC_LINUX_THREADS -D_GNU_SOURCE"
+		libdl="-ldl"
+		mono_cv_uscore=no
+		support_boehm=no
+		use_sigposix=yes
+		libgc_threads=pthreads
+		;;
 	*)
 		AC_MSG_WARN([*** Please add $host to configure.in checks!])
 		host_win32=no
@@ -365,6 +376,7 @@
 AM_CONDITIONAL(PLATFORM_DARWIN, test x$platform_darwin = xyes)
 AM_CONDITIONAL(PLATFORM_SIGPOSIX, test x$use_sigposix = xyes)
 AM_CONDITIONAL(PLATFORM_ANDROID, test x$platform_android = xyes)
+AM_CONDITIONAL(PLATFORM_RISCOS, test x$platform_riscos = xyes)
 
 AC_CHECK_TOOL(CC, gcc, gcc)
 AC_PROG_CC
@@ -670,7 +682,13 @@
 AC_CONFIG_SUBDIRS(eglib)
 
 GLIB_CFLAGS='-I$(top_srcdir)/eglib/src -I$(top_builddir)/eglib/src'
-GLIB_LIBS='-L$(top_builddir)/eglib/src -leglib -lm'
+if test x$platform_riscos = xyes; then
+   GLIB_STATIC_LIBS='-L$(top_builddir)/eglib/src -leglib-static'
+   GLIB_LIBS='-L$(top_builddir)/eglib/src -leglib'
+else
+   GLIB_STATIC_LIBS='-L$(top_builddir)/eglib/src -leglib -lm'
+   GLIB_LIBS='-L$(top_builddir)/eglib/src -leglib -lm'
+fi
 BUILD_GLIB_CFLAGS="$GLIB_CFLAGS"
 BUILD_GLIB_LIBS="$GLIB_LIBS"
 GMODULE_CFLAGS="$GLIB_CFLAGS"
@@ -678,6 +696,7 @@
   
 AC_SUBST(GLIB_CFLAGS)
 AC_SUBST(GLIB_LIBS)
+AC_SUBST(GLIB_STATIC_LIBS)
 AC_SUBST(GMODULE_CFLAGS)
 AC_SUBST(GMODULE_LIBS)
 AC_SUBST(BUILD_GLIB_CFLAGS)
@@ -2606,6 +2625,16 @@
 		AOT_SUPPORTED="yes"
 		CPPFLAGS="$CPPFLAGS -D__ARM_EABI__"
 		;;
+	arm*-riscos*)
+		TARGET=ARM;
+		arch_target=arm;
+		ACCESS_UNALIGNED="no"
+		JIT_SUPPORTED=yes
+		CPPFLAGS="$CPPFLAGS -DARM_FPU_NONE"
+		jit_wanted=true
+		sgen_supported=true
+		LIBC="libunixlib.so.5"
+		;;
 # TODO: make proper support for NaCl host.
 #        arm*-*nacl)
 #		TARGET=ARM;
@@ -3171,7 +3200,8 @@
 dnl
 dnl Consistency settings
 dnl
-if test x$cross_compiling = xyes -o x$enable_mcs_build = xno; then
+#if test x$cross_compiling = xyes -o x$enable_mcs_build = xno; then
+if test x$enable_mcs_build = xno; then
    DISABLE_MCS_DOCS=yes
    with_profile2=no
    with_profile4=no
@@ -3188,7 +3218,8 @@
 AC_SUBST(docs_dir)
 
 ## Maybe should also disable if mcsdir is invalid.  Let's punt the issue for now.
-AM_CONDITIONAL(BUILD_MCS, [test x$cross_compiling = xno && test x$enable_mcs_build != xno])
+#AM_CONDITIONAL(BUILD_MCS, [test x$cross_compiling = xno && test x$enable_mcs_build != xno])
+AM_CONDITIONAL(BUILD_MCS, [test x$enable_mcs_build != xno])
 
 AM_CONDITIONAL(HAVE_OPROFILE, test x$OPROFILE = xyes)
 AC_SUBST(OPROFILE_CFLAGS)
@@ -3506,18 +3537,21 @@
   # any existing config.make.  This allows people to share the same source tree
   # with different build directories, one native and one cross
   #
-  if test x$cross_compiling = xno && test x$enable_mcs_build != xno; then
+#  if test x$cross_compiling = xno && test x$enable_mcs_build != xno; then
 
     test -w $mcs_topdir/build || chmod +w $mcs_topdir/build
 
     echo "prefix=$prefix" > $mcs_topdir/build/config.make
     echo "exec_prefix=$exec_prefix" >> $mcs_topdir/build/config.make
     echo "sysconfdir=$sysconfdir" >> $mcs_topdir/build/config.make
-    echo 'mono_libdir=${exec_prefix}/lib' >> $mcs_topdir/build/config.make
+    echo 'mono_libdir=${prefix}/lib' >> $mcs_topdir/build/config.make
     echo 'IL_FLAGS = /debug' >> $mcs_topdir/build/config.make
-    echo "RUNTIME = $mono_build_root/runtime/mono-wrapper" >> $mcs_topdir/build/config.make
+    echo "RUNTIME = $mono_build_root/native-env/bin/mono" >> $mcs_topdir/build/config.make
     echo "ILDISASM = $mono_build_root/runtime/monodis-wrapper" >> $mcs_topdir/build/config.make
     echo "JAY_CFLAGS = $JAY_CFLAGS" >> $mcs_topdir/build/config.make
+    echo 'ifeq ($(thisdir),jay)' >> $mcs_topdir/build/config.make
+    echo 'CCOMPILE = /usr/bin/gcc $(USE_CFLAGS)' >> $mcs_topdir/build/config.make
+    echo "endif" >> $mcs_topdir/build/config.make
 
     case $INSTALL in
     [[\\/$]]* | ?:[[\\/]]* ) mcs_INSTALL=$INSTALL ;;
@@ -3586,7 +3620,9 @@
       echo "BCL_OPTIMIZE = 1" >> $srcdir/$mcsdir/build/config.make
     fi
 
-  fi
+    echo "BOOTSTRAP_MCS = PATH=$mono_build_root/native-env/bin gmcs" >> $mcs_topdir/build/config.make
+
+#  fi
 
   # if we have an olive folder, override the default settings
   if test -d $olivedir; then
