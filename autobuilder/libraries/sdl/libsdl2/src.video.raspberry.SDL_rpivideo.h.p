--- src/video/raspberry/SDL_rpivideo.h.orig	2016-01-02 19:56:31.000000000 +0000
+++ src/video/raspberry/SDL_rpivideo.h	2016-07-06 20:54:22.000000000 +0100
@@ -30,9 +30,22 @@
 #include "EGL/egl.h"
 #include "EGL/eglext.h"
 
+#ifdef __riscos__
+#include "oslib/os.h"
+#endif
+
 typedef struct SDL_VideoData
 {
     uint32_t egl_refcount;      /* OpenGL ES reference count              */
+#ifdef __riscos__
+    uint32_t eigen_x;
+    uint32_t eigen_y;
+    uint32_t screen_width;
+    uint32_t screen_height;
+    uint32_t old_mouse_buttons;
+#define RISCOS_MAX_KEYS_PRESSED 6
+    uint8_t key_pressed[RISCOS_MAX_KEYS_PRESSED];
+#endif
 } SDL_VideoData;
 
 
