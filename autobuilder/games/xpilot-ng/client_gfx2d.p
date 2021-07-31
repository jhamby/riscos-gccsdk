--- src/client/gfx2d.c.orig	2005-03-23 22:06:10.000000000 +0100
+++ src/client/gfx2d.c	2005-11-16 17:59:17.000000000 +0100
@@ -81,7 +85,13 @@
      */
     if (!strchr(filename, PATHNAME_SEP) && realTexturePath != NULL) {
 	for (dir = realTexturePath; *dir; dir = colon) {
-	    if (!(colon = strchr(dir, ':'))) {
+	    char *start;
+	    /* Go for next colon but only if it isn't directly followed by
+	       another one (RISC OS workaround for "/ADFS::DiscName/$/..."
+	       alike paths). */
+	    for (start = dir; (colon = strchr(start, ':')) && colon[1] == ':'; start = &colon[2])
+	      ;
+	    if (colon == NULL) {
 		len = strlen(dir);
 		colon = &dir[len];
 	    } else {
