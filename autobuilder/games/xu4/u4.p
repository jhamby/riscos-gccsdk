--- src/u4.cpp.orig	2017-07-04 12:52:01.945033000 +0100
+++ src/u4.cpp	2017-07-04 12:50:00.869033000 +0100
@@ -33,6 +33,12 @@
 #include "macosx/osxinit.h"
 #endif
 
+#if defined(__riscos__)
+#include "unixlib/local.h"
+/* Stop unixlib treating the zip files as directories */
+int __feature_imagefs_is_file = 1;
+#endif   
+
 bool verbose = false;
 bool quit = false;
 
@@ -149,5 +155,9 @@
     soundDelete();
     screenDelete();
 
+    // SDL_Quit in atexit causes an error on RISC OS when the timer has
+    // been started, so close the timer here to prevent the crash.
+    SDL_QuitSubSystem(SDL_INIT_TIMER);
+
     return 0;
 }
