--- lib/dpkg/parsehelp.c.orig	2011-05-07 16:02:32.734414616 +0100
+++ lib/dpkg/parsehelp.c	2011-05-07 16:03:15.857056519 +0100
@@ -32,6 +32,8 @@
 #include <dpkg/string.h>
 #include <dpkg/parsedump.h>
 
+#define isblank isspace
+
 static const char *
 parse_error_msg(struct parsedb_state *ps, const struct pkginfo *pigp,
                 const char *fmt)
