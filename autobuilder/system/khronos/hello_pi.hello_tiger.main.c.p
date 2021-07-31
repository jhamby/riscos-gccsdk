--- host_applications/linux/apps/hello_pi/hello_tiger/main.c.orig	2015-06-18 20:04:26.000000000 +0100
+++ host_applications/linux/apps/hello_pi/hello_tiger/main.c	2015-06-26 11:53:53.059613771 +0100
@@ -365,7 +365,8 @@
 	assert(eglGetError() == EGL_SUCCESS);
 	assert(numconfigs == 1);
 
-	eglsurface = eglCreateWindowSurface(egldisplay, eglconfig, window, NULL);
+	EGLint attribs[] = { EGL_RENDER_BUFFER, EGL_SINGLE_BUFFER, EGL_NONE, EGL_NONE };
+	eglsurface = eglCreateWindowSurface(egldisplay, eglconfig, window, attribs/*NULL*/);
 	assert(eglGetError() == EGL_SUCCESS);
 	eglcontext = eglCreateContext(egldisplay, eglconfig, NULL, NULL);
 	assert(eglGetError() == EGL_SUCCESS);
@@ -382,6 +383,8 @@
 	PS_destruct(tiger);
 	eglMakeCurrent(egldisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
 	assert(eglGetError() == EGL_SUCCESS);
+	eglDestroySurface(egldisplay, eglsurface);
+	eglDestroyContext(egldisplay, eglcontext);
 	eglTerminate(egldisplay);
 	assert(eglGetError() == EGL_SUCCESS);
 	eglReleaseThread();
@@ -480,6 +483,7 @@
 
 #elif defined __RASPBERRYPI__
 #include "bcm_host.h"
+#include <swis.h>
 int main(void)
 {
    uint32_t width, height;
@@ -519,11 +523,14 @@
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
 
