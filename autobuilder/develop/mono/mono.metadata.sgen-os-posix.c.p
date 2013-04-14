--- mono/metadata/sgen-os-posix.c.orig	2013-03-08 04:31:50.000000000 +0000
+++ mono/metadata/sgen-os-posix.c	2013-04-14 14:47:27.000000000 +0100
@@ -35,7 +35,7 @@
 #include "metadata/sgen-archdep.h"
 #include "metadata/object-internals.h"
 
-#if defined(__APPLE__) || defined(__OpenBSD__) || defined(__FreeBSD__)
+#if defined(__APPLE__) || defined(__OpenBSD__) || defined(__FreeBSD__) || defined(__riscos__)
 const static int suspend_signal_num = SIGXFSZ;
 #else
 const static int suspend_signal_num = SIGPWR;
@@ -183,6 +183,7 @@
 void
 sgen_wait_for_suspend_ack (int count)
 {
+#ifndef __riscos__
 	int i, result;
 
 	for (i = 0; i < count; ++i) {
@@ -192,6 +193,7 @@
 			}
 		}
 	}
+#endif
 }
 
 gboolean
