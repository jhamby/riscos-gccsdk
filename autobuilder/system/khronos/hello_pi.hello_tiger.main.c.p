--- host_applications/linux/apps/hello_pi/hello_tiger/main.c.orig	2022-11-23 17:52:43.936646162 +0000
+++ host_applications/linux/apps/hello_pi/hello_tiger/main.c	2022-11-23 17:52:44.064647585 +0000
@@ -367,7 +367,8 @@
 	assert(eglGetError() == EGL_SUCCESS);
 	assert(numconfigs == 1);
 
-	eglsurface = eglCreateWindowSurface(egldisplay, eglconfig, window, NULL);
+	EGLint attribs[] = { EGL_RENDER_BUFFER, EGL_SINGLE_BUFFER, EGL_NONE, EGL_NONE };
+	eglsurface = eglCreateWindowSurface(egldisplay, eglconfig, window, attribs/*NULL*/);
 	assert(eglGetError() == EGL_SUCCESS);
 	eglcontext = eglCreateContext(egldisplay, eglconfig, NULL, NULL);
 	assert(eglGetError() == EGL_SUCCESS);
@@ -384,6 +385,8 @@
 	PS_destruct(tiger);
 	eglMakeCurrent(egldisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
 	assert(eglGetError() == EGL_SUCCESS);
+	eglDestroySurface(egldisplay, eglsurface);
+	eglDestroyContext(egldisplay, eglcontext);
 	eglTerminate(egldisplay);
 	assert(eglGetError() == EGL_SUCCESS);
 	eglReleaseThread();
@@ -482,6 +485,7 @@
 
 #elif defined __RASPBERRYPI__
 #include "bcm_host.h"
+#include <swis.h>
 int main(void)
 {
    uint32_t width, height;
@@ -528,11 +532,14 @@
    nativewindow.height = height;
    vc_dispmanx_update_submit_sync( dispman_update );
 
+   atexit(deinit);
    init(&nativewindow);
 
    while (1) {
       render(width, height);
       rotateN += 1.0f;
+      if (_swi(OS_Byte,_IN(0)|_RETURN(1),122) != 0xff)
+         break;
    }
    deinit();
 
