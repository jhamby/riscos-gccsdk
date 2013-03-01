--- libgc/configure.in.orig	2013-01-08 18:41:10.000000000 +0000
+++ libgc/configure.in	2013-01-16 19:41:46.000000000 +0000
@@ -168,6 +168,9 @@
 	AM_CPPFLAGS="$AM_CPPFLAGS -pthread"
 	THREADDLLIBS="-lpthread -lrt"
 	;;
+     *-*-riscos*)
+	AC_DEFINE(GC_LINUX_THREADS)
+	;;
       *)
 	AC_MSG_ERROR("Pthreads not supported by the GC on this platform.")
 	;;
