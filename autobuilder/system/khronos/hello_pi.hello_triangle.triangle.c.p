--- host_applications/linux/apps/hello_pi/hello_triangle/triangle.c.orig	2022-11-23 17:47:07.056363390 +0000
+++ host_applications/linux/apps/hello_pi/hello_triangle/triangle.c	2022-11-23 17:47:07.184365269 +0000
@@ -33,6 +33,7 @@
 #include <math.h>
 #include <assert.h>
 #include <unistd.h>
+#include <swis.h>
 
 #include "bcm_host.h"
 
@@ -44,7 +45,7 @@
 
 #include "revision.h"
 
-#define PATH "./"
+#define PATH "/<Khronos$Test>/"
 
 #define IMAGE_SIZE 128
 
@@ -171,8 +172,9 @@
    nativewindow.width = state->screen_width;
    nativewindow.height = state->screen_height;
    vc_dispmanx_update_submit_sync( dispman_update );
-      
-   state->surface = eglCreateWindowSurface( state->display, config, &nativewindow, NULL );
+   
+   EGLint attribs[] = { EGL_RENDER_BUFFER, EGL_SINGLE_BUFFER, EGL_NONE, EGL_NONE };
+   state->surface = eglCreateWindowSurface( state->display, config, &nativewindow, attribs/*NULL*/ );
    assert(state->surface != EGL_NO_SURFACE);
 
    // connect the context to the surface
@@ -180,7 +182,7 @@
    assert(EGL_FALSE != result);
 
    // Set background color and clear buffers
-   glClearColor(0.15f, 0.25f, 0.35f, 1.0f);
+   glClearColor(0.15f, 0.25f, 0.35f, 0.5f/*1.0f*/);
 
    // Enable back face culling.
    glEnable(GL_CULL_FACE);
@@ -548,12 +550,16 @@
    // initialise the OGLES texture(s)
    init_textures(state);
 
+   atexit(exit_func);
+
    while (!terminate)
    {
       update_model(state);
       redraw_scene(state);
+      if (_swi(OS_Byte,_IN(0)|_RETURN(1),122) != 0xff)
+        terminate = 1;
    }
-   exit_func();
+
    return 0;
 }
 
