--- client/gui-sdl/themespec.c.orig	2008-06-16 08:48:04.000000000 +0100
+++ client/gui-sdl/themespec.c	2008-09-15 13:21:35.198736600 +0100
@@ -732,7 +732,22 @@
   } else {
     c = secfile_lookup_str(file, "themespec.font_file");
   }
+#ifdef __riscos__
+  /* Need to get RISC OS font from UnixFont directory */
+  {
+     static char riscos_fontpath[256];
+     char *p = strstr(c, "truetype");
+     if (p)
+     {
+        strcpy(riscos_fontpath, "/UnixFont:");
+        strcat(riscos_fontpath, p);
+        t->font_filename = riscos_fontpath;
+     } else
+        t->font_filename = 0;
+  }        
+#else
   t->font_filename = datafilename(c);
+#endif
   if (t->font_filename) {
     t->font_filename = mystrdup(t->font_filename);
   } else {
