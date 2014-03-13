--- src/settings.cpp.orig	2005-08-22 06:53:38.000000000 +0100
+++ src/settings.cpp	2014-03-05 11:12:54.000000000 +0000
@@ -71,6 +71,15 @@
         userPath += "/";
     } else
         userPath = "./";
+#elif defined(__riscos__)
+    char *home = getenv("Choices$Write");
+    if (!home) home = getenv("HOME");
+    if (home && home[0]) {
+        userPath += home;
+        userPath += "/.xu4";
+        userPath += "/";
+    } else
+        userPath = "./";
 #elif defined(__unix__)
     char *home = getenv("HOME");
     if (home && home[0]) {
