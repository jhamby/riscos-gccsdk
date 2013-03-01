--- mono/metadata/sgen-os-posix.c.orig	2013-01-08 18:41:08.000000000 +0000
+++ mono/metadata/sgen-os-posix.c	2013-01-18 17:59:14.000000000 +0000
@@ -35,7 +35,7 @@
 #include "metadata/sgen-archdep.h"
 #include "metadata/object-internals.h"
 
-#if defined(__APPLE__) || defined(__OpenBSD__) || defined(__FreeBSD__)
+#if defined(__APPLE__) || defined(__OpenBSD__) || defined(__FreeBSD__) || defined(__riscos__)
 const static int suspend_signal_num = SIGXFSZ;
 #else
 const static int suspend_signal_num = SIGPWR;
@@ -185,6 +185,7 @@
 void
 sgen_wait_for_suspend_ack (int count)
 {
+#ifndef __riscos__
 	int i, result;
 
 	for (i = 0; i < count; ++i) {
@@ -194,6 +195,7 @@
 			}
 		}
 	}
+#endif
 }
 
 gboolean
