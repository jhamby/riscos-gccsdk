--- src/util/system.cpp.orig	2011-09-13 08:47:42.000000000 +0100
+++ src/util/system.cpp	2011-09-13 09:31:41.000000000 +0100
@@ -19,7 +19,6 @@
 #include <iostream>
 #include <sstream>
 #include <stdexcept>
-#include <string.h>
 
 #ifndef WIN32
 #  include <dirent.h>
@@ -347,9 +346,6 @@
     lang = lang_c;
 
   if (lang.empty() || lang == "C")
-    lang = getenv("LANG");
-
-  if (lang.empty() || lang == "C")
     return globals::default_language;
   else
     return lang.substr(0, 2);
@@ -382,7 +378,12 @@
   {
     bytes_read = fread(buffer, sizeof (char), 4096, in);
 
+#ifdef __riscos__
+	/* GCG 4.1.1 on RISC OS did not set feof */
+    if (bytes_read != 4096 && ferror(in))
+#else
     if (bytes_read != 4096 && !feof(in))
+#endif
     {
       throw std::runtime_error("System:checksum: file read error");
     }
@@ -440,6 +441,9 @@
 #endif
   else
   {
+#ifdef __riscos__
+#define PATH_MAX 256
+#endif
     char buf[PATH_MAX];
     if (getcwd(buf, PATH_MAX) == 0)
     {
