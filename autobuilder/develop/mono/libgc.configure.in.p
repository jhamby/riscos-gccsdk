--- libgc/configure.in.orig	2011-08-26 02:06:38.000000000 +0100
+++ libgc/configure.in	2011-11-13 14:44:40.000000000 +0000
@@ -165,6 +165,9 @@
 	INCLUDES="$INCLUDES -pthread"
 	THREADDLLIBS="-lpthread -lrt"
 	;;
+     *-*-riscos*)
+	AC_DEFINE(GC_LINUX_THREADS)
+	;;
       *)
 	AC_MSG_ERROR("Pthreads not supported by the GC on this platform.")
 	;;
