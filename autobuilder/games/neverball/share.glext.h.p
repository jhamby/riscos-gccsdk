--- share/glext.h.orig	2015-08-22 14:15:39.000000000 +0100
+++ share/glext.h	2015-08-22 14:44:36.885726596 +0100
@@ -25,6 +25,8 @@
 
 #ifdef __APPLE__
 #include <OpenGL/gl.h>
+#elif defined (__riscos__)
+#include <GLES/gl.h>
 #else
 #include <GL/gl.h>
 #endif
