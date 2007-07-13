--- src/serialization/string_utils.cpp.orig	2007-02-06 15:50:12.000000000 +0000
+++ src/serialization/string_utils.cpp	2007-07-11 16:14:03.204745300 +0100
@@ -15,6 +15,14 @@
 
 #include "global.hpp"
 
+#ifdef __riscos__
+// Fix for problem with mbstate_t include in RISC OS
+// port of GCC 3.4.6 compiler
+#define _GLIBCXX_HAVE_MBSTATE_T 1
+#include "wchar.h"
+#endif
+
+
 #include <cctype>
 #include <sstream>
 #include "SDL_types.h"
