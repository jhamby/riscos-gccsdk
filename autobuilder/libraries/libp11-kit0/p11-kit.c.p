--- tools/p11-kit.c.orig	2011-09-23 19:27:13.000000000 +0100
+++ tools/p11-kit.c	2011-09-23 19:27:35.000000000 +0100
@@ -44,6 +46,17 @@
 #include "p11-kit/p11-kit.h"
 #include "p11-kit/uri.h"

+#define warnx(...) do { \
+	fprintf (stderr, __VA_ARGS__); \
+	fprintf (stderr, "\n"); \
+} while (0)
+
+#define errx(code, ...) do { \
+	fprintf (stderr, __VA_ARGS__); \
+	fprintf (stderr, "\n"); \
+	exit (code); \
+} while (0)
+
 typedef int (*operation) (int argc, char *argv[]);
 int verbose = 0;

