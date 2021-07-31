--- src/video/SDL_egl.c.orig	2017-11-26 09:29:07.341380436 +0000
+++ src/video/SDL_egl.c	2017-11-26 09:36:26.418068053 +0000
@@ -43,6 +43,14 @@
 #endif /* EGL_KHR_create_context */
 
 #if SDL_VIDEO_DRIVER_RPI
+
+#  if __riscos__
+#    define DEFAULT_EGL "/SharedLibs:lib/abi-2.0/vfp/libEGL.so"
+#    define DEFAULT_OGL_ES2 "/SharedLibs:lib/abi-2.0/vfp/libGLESv2.so"
+/* These two don't actually exist on RISC OS.  */
+#    define DEFAULT_OGL_ES_PVR "/SharedLibs:lib/abi-2.0/vfp/libGLES_CM.so"
+#    define DEFAULT_OGL_ES "/SharedLibs:lib/abi-2.0/vfp/libGLESv1_CM.so"
+#  else
 /* Raspbian places the OpenGL ES/EGL binaries in a non standard path */
 #define DEFAULT_EGL "/opt/vc/lib/libbrcmEGL.so"
 #define DEFAULT_OGL_ES2 "/opt/vc/lib/libbrcmGLESv2.so"
@@ -50,6 +58,7 @@
 #define ALT_OGL_ES2 "/opt/vc/lib/libGLESv2.so"
 #define DEFAULT_OGL_ES_PVR "/opt/vc/lib/libGLES_CM.so"
 #define DEFAULT_OGL_ES "/opt/vc/lib/libGLESv1_CM.so"
+#  endif
 
 #elif SDL_VIDEO_DRIVER_ANDROID || SDL_VIDEO_DRIVER_VIVANTE
 /* Android */
@@ -781,11 +790,12 @@
         ANativeWindow_setBuffersGeometry(nw, 0, 0, format);
     }
 #endif    
-    
+
+    EGLint attribs[] = { EGL_RENDER_BUFFER, EGL_SINGLE_BUFFER, EGL_NONE, EGL_NONE };    
     surface = _this->egl_data->eglCreateWindowSurface(
             _this->egl_data->egl_display,
             _this->egl_data->egl_config,
-            nw, NULL);
+            nw, attribs/*NULL*/);
     if (surface == EGL_NO_SURFACE) {
         SDL_EGL_SetError("unable to create an EGL window surface", "eglCreateWindowSurface");
     }
