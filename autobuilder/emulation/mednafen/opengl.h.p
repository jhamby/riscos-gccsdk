--- src/drivers/opengl.h.orig	2012-03-27 14:37:01.000000000 +0100
+++ src/drivers/opengl.h	2012-03-27 14:37:26.000000000 +0100
@@ -1,3 +1,4 @@
+#ifndef __riscos__
 #ifndef __DRIVERS_OPENGL_H
 #define __DRIVERS_OPENGL_H
 
@@ -176,3 +177,4 @@
 void FlipOpenGL(void);
 
 #endif
+#endif
