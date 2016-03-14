--- compat/unix/paths.c.orig	2016-03-13 12:23:26.914075498 +0000
+++ compat/unix/paths.c	2016-03-13 12:24:01.294075533 +0000
@@ -112,7 +112,7 @@
     return 1;
 
   case 3:
-    strncpy( ctx->path, "/usr/local/share/spectrum-roms", PATH_MAX );
+    strncpy( ctx->path, "/<Fuse$Dir>", PATH_MAX );
     return 1;
 
     /* Then where we may have installed the data files */
