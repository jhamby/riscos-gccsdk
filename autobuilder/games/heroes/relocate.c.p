--- src/relocate.c.orig	2014-03-14 08:57:15.000000000 +0000
+++ src/relocate.c	2014-03-14 08:57:31.000000000 +0000
@@ -62,8 +62,21 @@
   dmsg (D_SYSTEM, "looking for HEROES_HOME_DIR, HEROES_HOMEDIR or HOME...");
   if ((home_dir = getenv ("HEROES_HOME_DIR")) ||
       (home_dir = getenv ("HEROES_HOMEDIR")) ||
+#ifdef __riscos__
+      (home_dir = getenv ("Choices$Write")) ||
+#endif
       (home_dir = getenv ("HOME"))) {
     dmsg (D_SYSTEM,"... found: %s", home_dir);
+#ifdef __riscos__
+    if (home_dir[0] != '/')
+    {
+       char tmp[256];
+// Ensure name is in unix format or create of choices directory will fail later
+       strcpy(tmp, "/");
+       strcat(tmp, home_dir);
+       set_rsc_file ("home-dir", tmp, false);
+    } else
+#endif
     set_rsc_file ("home-dir", home_dir, false);
   } else {
     dmsg (D_SYSTEM, "... not found.");
