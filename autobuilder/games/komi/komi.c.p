--- komi.c.orig	2014-03-27 12:02:37.000000000 +0000
+++ komi.c	2014-03-27 12:34:59.000000000 +0000
@@ -3834,6 +3834,19 @@
 {
    char * dir = NULL;
 
+#ifdef __riscos__
+   // RISC OS has Choices
+   if ((dir = getenv("Choices$Write")) != NULL)
+   {
+      if (strlen(dir) + 2 >= sizeof(prefsdir))
+      {
+         fprintf(stderr, "Could not set prefs directory - name is too long:\n%s\n", dir);
+      } else {
+         sprintf(prefsdir, "/%s/", dir);
+      }
+      return;
+   }
+#endif
    // Unix has $HOME
    if ((dir = getenv("HOME")) != NULL)
    {
