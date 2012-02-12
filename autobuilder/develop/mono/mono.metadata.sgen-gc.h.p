--- mono/metadata/sgen-gc.h.orig	2011-11-14 17:53:03.000000000 +0000
+++ mono/metadata/sgen-gc.h	2011-11-14 17:53:30.000000000 +0000
@@ -199,7 +199,7 @@
 
 typedef struct _SgenPinnedChunk SgenPinnedChunk;
 
-#if defined(__APPLE__) || defined(__OpenBSD__) || defined(__FreeBSD__)
+#if defined(__APPLE__) || defined(__OpenBSD__) || defined(__FreeBSD__) || defined(__riscos__)
 const static int suspend_signal_num = SIGXFSZ;
 #else
 const static int suspend_signal_num = SIGPWR;
