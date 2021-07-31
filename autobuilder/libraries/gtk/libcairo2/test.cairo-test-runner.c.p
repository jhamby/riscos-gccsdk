--- test/cairo-test-runner.c.orig	2012-02-02 00:48:14.000000000 +0000
+++ test/cairo-test-runner.c	2013-04-16 20:09:32.000000000 +0100
@@ -36,6 +36,9 @@
 #include <pixman.h> /* for version information */
 
 #define SHOULD_FORK HAVE_FORK && HAVE_WAITPID
+#ifdef __riscos__
+#undef SHOULD_FORK
+#endif
 #if SHOULD_FORK
 #if HAVE_UNISTD_H
 #include <unistd.h>
