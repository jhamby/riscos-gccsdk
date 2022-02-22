--- gdk/gdkwindow.c.orig	2020-09-03 22:56:40.000000000 +0100
+++ gdk/gdkwindow.c	2020-11-22 21:12:47.086366252 +0000
@@ -45,7 +45,9 @@
 
 #include <math.h>
 
+#ifndef GDK_WINDOWING_RISCOS
 #include <epoxy/gl.h>
+#endif
 
 /* for the use of round() */
 #include "fallback-c89.c"
@@ -2964,6 +2966,7 @@
 
   window->current_paint.use_gl = window->impl_window->gl_paint_context != NULL;
 
+#ifndef GDK_WINDOWING_RISCOS
   if (window->current_paint.use_gl)
     {
       GdkGLContext *context;
@@ -2995,6 +2998,7 @@
           glViewport (0, 0, ww, wh);
         }
     }
+#endif
 
   if (needs_surface)
     {
@@ -3048,6 +3052,7 @@
 
       cairo_region_get_extents (window->current_paint.region, &clip_box);
 
+#ifndef GDK_WINDOWING_RISCOS
       if (window->current_paint.use_gl)
         {
           cairo_region_t *opaque_region = cairo_region_copy (window->current_paint.region);
@@ -3074,6 +3079,7 @@
                                     window->active_update_area);
         }
       else
+#endif
         {
           surface = gdk_window_ref_impl_surface (window);
           cr = cairo_create (surface);
