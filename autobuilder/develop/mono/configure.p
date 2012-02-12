--- configure.orig	2011-09-15 18:12:44.000000000 +0100
+++ configure	2011-11-15 12:10:16.000000000 +0000
@@ -3979,6 +3979,14 @@
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
 	*-*-nacl*)
 		host_win32=no
 		CPPFLAGS="$CPPFLAGS -DGC_LINUX_THREADS -D_GNU_SOURCE -D_REENTRANT -DUSE_MMAP"
@@ -23089,6 +23097,15 @@
 		jit_wanted=true
 		sgen_supported=true
 		;;
+	arm*-riscos*)
+		TARGET=ARM;
+		arch_target=arm;
+		ACCESS_UNALIGNED="no"
+		JIT_SUPPORTED=yes
+		CPPFLAGS="$CPPFLAGS -DARM_FPU_FPA"
+		jit_wanted=true
+		sgen_supported=true
+		;;
 	s390-*-linux*)
 		TARGET=S390;
 		arch_target=s390;
