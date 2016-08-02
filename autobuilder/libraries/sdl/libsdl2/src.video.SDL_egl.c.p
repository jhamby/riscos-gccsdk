--- src/video/SDL_egl.c.orig	2016-01-02 19:56:31.000000000 +0000
+++ src/video/SDL_egl.c	2016-06-22 11:07:29.310465071 +0100
@@ -39,11 +39,20 @@
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
 #define DEFAULT_EGL "/opt/vc/lib/libEGL.so"
 #define DEFAULT_OGL_ES2 "/opt/vc/lib/libGLESv2.so"
 #define DEFAULT_OGL_ES_PVR "/opt/vc/lib/libGLES_CM.so"
 #define DEFAULT_OGL_ES "/opt/vc/lib/libGLESv1_CM.so"
+#  endif
 
 #elif SDL_VIDEO_DRIVER_ANDROID || SDL_VIDEO_DRIVER_VIVANTE
 /* Android */
@@ -608,10 +617,11 @@
     }
 #endif    
     
+    EGLint attribs[] = { EGL_RENDER_BUFFER, EGL_SINGLE_BUFFER, EGL_NONE, EGL_NONE };
     return _this->egl_data->eglCreateWindowSurface(
             _this->egl_data->egl_display,
             _this->egl_data->egl_config,
-            nw, NULL);
+            nw, attribs/*NULL*/);
 }
 
 void
