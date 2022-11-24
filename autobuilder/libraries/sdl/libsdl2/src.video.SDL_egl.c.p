--- src/video/SDL_egl.c.orig	2022-11-23 22:42:11.777073000 +0000
+++ src/video/SDL_egl.c	2022-11-23 22:43:31.126162366 +0000
@@ -58,6 +58,14 @@
 #endif /* EGL_EXT_present_opaque */
 
 #if SDL_VIDEO_DRIVER_RPI
+
+#  if __RISCOS__
+#    define DEFAULT_EGL "libEGL.so"
+#    define DEFAULT_OGL_ES2 "libGLESv2.so"
+/* These two don't actually exist on RISC OS.  */
+#    define DEFAULT_OGL_ES_PVR "libGLES_CM.so"
+#    define DEFAULT_OGL_ES "libGLESv1_CM.so"
+#  else
 /* Raspbian places the OpenGL ES/EGL binaries in a non standard path */
 #define DEFAULT_EGL ( vc4 ? "libEGL.so.1" : "libbrcmEGL.so" )
 #define DEFAULT_OGL_ES2 ( vc4 ? "libGLESv2.so.2" : "libbrcmGLESv2.so" )
@@ -65,6 +73,7 @@
 #define ALT_OGL_ES2 "libGLESv2.so"
 #define DEFAULT_OGL_ES_PVR ( vc4 ? "libGLES_CM.so.1" : "libbrcmGLESv2.so" )
 #define DEFAULT_OGL_ES ( vc4 ? "libGLESv1_CM.so.1" : "libbrcmGLESv2.so" )
+#endif
 
 #elif SDL_VIDEO_DRIVER_ANDROID || SDL_VIDEO_DRIVER_VIVANTE
 /* Android */
@@ -1200,8 +1209,8 @@
     EGLint format_wanted;
     EGLint format_got;
 #endif
-    /* max 2 key+value pairs, plus terminator. */
-    EGLint attribs[5];
+    /* max 3 key+value pairs, plus terminator. */
+    EGLint attribs[7];
     int attr = 0;
 
     EGLSurface * surface;
@@ -1242,6 +1251,11 @@
     }
 #endif
 
+#if defined(__RISCOS__) && defined(SDL_VIDEO_DRIVER_RPI)
+    attribs[attr++] = EGL_RENDER_BUFFER;
+    attribs[attr++] = EGL_SINGLE_BUFFER;
+#endif
+
     attribs[attr++] = EGL_NONE;
     
     surface = _this->egl_data->eglCreateWindowSurface(
