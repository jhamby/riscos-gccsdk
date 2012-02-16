--- configure.orig	2011-12-19 21:16:07.000000000 +0000
+++ configure	2012-02-13 19:22:14.000000000 +0000
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
@@ -23105,6 +23113,15 @@
 		# Required CFLAGS for s390[x].  USE_STRING_INLINES is automatic with gcc 4.1
 		CFLAGS="$CFLAGS -mbackchain -D__USE_STRING_INLINES"
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
 	s390x-*-linux*)
 		TARGET=S390x;
 		arch_target=s390x;
