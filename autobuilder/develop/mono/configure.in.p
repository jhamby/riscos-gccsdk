--- configure.in.orig	2011-12-19 21:09:17.000000000 +0000
+++ configure.in	2012-02-21 15:51:48.000000000 +0000
@@ -313,6 +313,14 @@
 		libgc_threads=pthreads
 		use_sigposix=yes
 		;;
+	*-*-riscos*)
+		host_win32=no
+		CPPFLAGS="$CPPFLAGS -DGC_LINUX_THREADS -D_GNU_SOURCE"
+		libdl="-ldl"
+		mono_cv_uscore=no
+		support_boehm=no
+		use_sigposix=yes
+		;;
 	*)
 		AC_MSG_WARN([*** Please add $host to configure.in checks!])
 		host_win32=no
@@ -2345,6 +2353,16 @@
 		jit_wanted=true
 		sgen_supported=true
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
 	s390-*-linux*)
 		TARGET=S390;
 		arch_target=s390;
