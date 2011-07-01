--- src/clipboard.cpp.orig	2010-08-30 19:15:47.000000000 +0100
+++ src/clipboard.cpp	2011-04-05 16:01:25.041403200 +0100
@@ -19,7 +19,7 @@
 #include "clipboard.hpp"
 #include <algorithm>
 
-#if defined(_X11) && !defined(__APPLE__)
+#if defined(_X11) && !defined(__APPLE__) && !defined(__riscos__)
 
 #define CLIPBOARD_FUNCS_DEFINED
 
