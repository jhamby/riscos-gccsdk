--- ./src/engine/display/screenshot.cpp.orig	2011-09-08 13:55:20.558976900 +0100
+++ ./src/engine/display/screenshot.cpp	2011-09-08 14:13:14.800875700 +0100
@@ -20,6 +20,10 @@
 #include <iostream>
 #include <png.h>
 
+#ifdef __riscos__
+#include "config.h"
+#endif
+
 #include "pingus/gettext.h"
 #include "util/system.hpp"
 
