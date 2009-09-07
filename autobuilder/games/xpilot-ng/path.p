--- src/client/option.c.orig	2009-09-07 14:45:21.000000000 -0700
+++ src/client/option.c	2009-09-07 14:51:55.000000000 -0700
@@ -1167,6 +1167,12 @@
 }
 
 #ifndef _WINDOWS
+#if defined(__riscos__)
+void Xpilotrc_get_filename(char *path, size_t size)
+{
+    strlcpy(path, "/<Choices$Write>/xpilotrc", size);
+}
+#else
 void Xpilotrc_get_filename(char *path, size_t size)
 {
     const char *home = getenv("HOME");
@@ -1180,8 +1186,9 @@
 	strlcat(path, "/", size);
 	strlcat(path, defaultFile, size);
     } else
-	strlcpy(path, "", size);
+       strlcpy(path, "", size);
 }
+#endif
 #else
 void Xpilotrc_get_filename(char *path, size_t size)
 {
