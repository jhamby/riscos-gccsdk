--- configure.in.orig	2013-03-08 04:31:06.000000000 +0000
+++ configure.in	2013-04-14 14:47:27.000000000 +0100
@@ -339,6 +339,14 @@
 		libgc_threads=pthreads
 		use_sigposix=yes
 		;;
+	*-*-riscos*)
+		host_win32=no
+		CPPFLAGS="$CPPFLAGS -DGC_LINUX_THREADS -D_GNU_SOURCE -Xassembler -mno-warn-deprecated"
+		libdl="-ldl"
+		mono_cv_uscore=no
+		support_boehm=no
+		use_sigposix=yes
+		;;
 	*)
 		AC_MSG_WARN([*** Please add $host to configure.in checks!])
 		host_win32=no
@@ -2558,6 +2566,16 @@
 		   NESTED_LIBGC_FLAGS="$NESTED_LIBGC_FLAGS -DHAVE_ARMV6"
   		fi
 		;;
+	arm*-riscos*)
+		TARGET=ARM;
+		arch_target=arm;
+		ACCESS_UNALIGNED="no"
+		JIT_SUPPORTED=yes
+		CPPFLAGS="$CPPFLAGS -DARM_FPU_NONE -Xassembler -mno-warn-deprecated"
+		jit_wanted=true
+		sgen_supported=true
+		LIBC="libunixlib.so.5"
+		;;
 	s390-*-linux*)
 		TARGET=S390;
 		arch_target=s390;
@@ -3076,7 +3094,8 @@
 dnl
 dnl Consistency settings
 dnl
-if test x$cross_compiling = xyes -o x$enable_mcs_build = xno; then
+#if test x$cross_compiling = xyes -o x$enable_mcs_build = xno; then
+if test x$enable_mcs_build = xno; then
    DISABLE_MCS_DOCS=yes
    with_profile2=no
    with_profile4=no
@@ -3093,7 +3112,8 @@
 AC_SUBST(docs_dir)
 
 ## Maybe should also disable if mcsdir is invalid.  Let's punt the issue for now.
-AM_CONDITIONAL(BUILD_MCS, [test x$cross_compiling = xno && test x$enable_mcs_build != xno])
+#AM_CONDITIONAL(BUILD_MCS, [test x$cross_compiling = xno && test x$enable_mcs_build != xno])
+AM_CONDITIONAL(BUILD_MCS, [test x$enable_mcs_build != xno])
 
 AM_CONDITIONAL(HAVE_OPROFILE, test x$OPROFILE = xyes)
 AC_SUBST(OPROFILE_CFLAGS)
@@ -3408,18 +3428,21 @@
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
@@ -3488,7 +3511,9 @@
       echo "BCL_OPTIMIZE = 1" >> $srcdir/$mcsdir/build/config.make
     fi
 
-  fi
+    echo "BOOTSTRAP_MCS = PATH=$mono_build_root/native-env/bin gmcs" >> $mcs_topdir/build/config.make
+
+#  fi
 
   # if we have an olive folder, override the default settings
   if test -d $olivedir; then
