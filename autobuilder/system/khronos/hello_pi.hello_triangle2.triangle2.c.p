--- host_applications/linux/apps/hello_pi/hello_triangle2/triangle2.c.orig	2015-06-22 20:29:42.000000000 +0100
+++ host_applications/linux/apps/hello_pi/hello_triangle2/triangle2.c	2015-06-26 12:18:16.347655735 +0100
@@ -42,6 +42,8 @@
 #include "EGL/egl.h"
 #include "EGL/eglext.h"
 
+#include <swis.h>
+
 typedef struct
 {
    uint32_t screen_width;
@@ -181,7 +183,8 @@
       
    check();
 
-   state->surface = eglCreateWindowSurface( state->display, config, &nativewindow, NULL );
+   EGLint attribs[] = { EGL_RENDER_BUFFER, EGL_SINGLE_BUFFER, EGL_NONE, EGL_NONE };
+   state->surface = eglCreateWindowSurface( state->display, config, &nativewindow, attribs/*NULL*/ );
    assert(state->surface != EGL_NO_SURFACE);
    check();
 
@@ -441,8 +444,44 @@
         check();
 }
 
+static int mouse(int *outx, int *outy)
+{
+  register int x __asm("r0");
+  register int y __asm("r1");
+  register int b __asm("r2");
+
+  __asm volatile ("SWI	%[SWI_OS_Mouse];\n\t"
+		  : "=r" (x), "=r" (y), "=r" (b)
+		  : [SWI_OS_Mouse] "i" (OS_Mouse));
+  if (outx)
+    *outx = x >> 1; /* Crude conversion to pixels.  */
+  if (outy)
+    *outy = abs(y) >> 1;
+  return b;
+}
+
 static int get_mouse(CUBE_STATE_T *state, int *outx, int *outy)
 {
+#ifdef __riscos__
+    const int width=state->screen_width, height=state->screen_height;
+    static int x=800, y=400;
+    int b;
+
+    b = mouse(&x, &y);
+    if (b & 2)
+      return 1;
+    if (b & 1)
+      x-=256;
+    if (b & 4)
+      y-=256;
+    if (x<0) x=0;
+    if (y<0) y=0;
+    if (x>width) x=width;
+    if (y>height) y=height;
+    if (outx) *outx = x;
+    if (outy) *outy = y;
+    return 0;
+#else
     static int fd = -1;
     const int width=state->screen_width, height=state->screen_height;
     static int x=800, y=400;
@@ -477,8 +516,24 @@
    if (outx) *outx = x;
    if (outy) *outy = y;
    return 0;
-}       
- 
+#endif
+}
+
+static void exit_func(void)
+{
+   // clear screen
+   glClear( GL_COLOR_BUFFER_BIT );
+   eglSwapBuffers(state->display, state->surface);
+
+   // Release OpenGL resources
+   eglMakeCurrent( state->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT );
+   eglDestroySurface( state->display, state->surface );
+   eglDestroyContext( state->display, state->context );
+   eglTerminate( state->display );
+
+   printf("\nFinished\n");
+} // exit_func()
+
 //==============================================================================
 
 int main ()
@@ -504,6 +559,9 @@
       if (b) break;
       draw_triangles(state, cx, cy, 0.003, x, y);
    }
+   
+   exit_func();
+   
    return 0;
 }
 
