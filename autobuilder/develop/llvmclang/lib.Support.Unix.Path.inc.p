--- lib/Support/Unix/Path.inc.orig	2014-06-30 19:57:04.000000000 +0100
+++ lib/Support/Unix/Path.inc	2015-02-01 20:09:26.952265610 +0000
@@ -108,7 +108,24 @@
 namespace fs {
 #if defined(__FreeBSD__) || defined (__NetBSD__) || defined(__Bitrig__) || \
     defined(__OpenBSD__) || defined(__minix) || defined(__FreeBSD_kernel__) || \
-    defined(__linux__) || defined(__CYGWIN__) || defined(__DragonFly__)
+    defined(__linux__) || defined(__CYGWIN__) || defined(__DragonFly__) || \
+    defined(__riscos__)
+#ifdef __riscos__
+static int
+test_dir(char ret[PATH_MAX], const char *dir, const char *bin)
+{  
+  struct stat sb;
+
+  if (strcmp(dir, "/") == 0)
+    snprintf(ret, PATH_MAX, "%s", bin);
+  else
+    snprintf(ret, PATH_MAX, "%s/%s", dir, bin);
+  if (stat(ret, &sb) != 0)
+    return (1);
+
+  return (0);
+}
+#else
 static int
 test_dir(char ret[PATH_MAX], const char *dir, const char *bin)
 {  
@@ -123,6 +140,7 @@
 
   return (0);
 }
+#endif
 
 static char *
 getprogpath(char ret[PATH_MAX], const char *bin)
@@ -184,7 +202,7 @@
 
   if (getprogpath(exe_path, argv0) != NULL)
     return exe_path;
-#elif defined(__linux__) || defined(__CYGWIN__)
+#elif defined(__linux__) || defined(__CYGWIN__) || defined(__riscos__)
   char exe_path[MAXPATHLEN];
   StringRef aPath("/proc/self/exe");
   if (sys::fs::exists(aPath)) {
