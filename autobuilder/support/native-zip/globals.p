--- globals.c.orig	2005-03-20 12:32:02.000000000 -0800
+++ globals.c	2008-01-12 22:45:07.000000000 -0800
@@ -28,6 +28,10 @@
 int pathput = 1;        /* 1=store path with name */
 #ifdef RISCOS
 int scanimage = 1;      /* 1=scan through image files */
+#else
+#ifdef FORRISCOS
+int decomma = 0;
+#endif
 #endif
 int method = BEST;      /* one of BEST, DEFLATE (only), or STORE (only) */
 int dosify = 0;         /* 1=make new entries look like MSDOS */
