--- gdk/gdkgl.c.orig	2020-06-11 03:45:39.000000000 +0100
+++ gdk/gdkgl.c	2020-11-22 20:43:50.541195975 +0000
@@ -26,7 +26,9 @@
 # include "win32/gdkwin32.h"
 #endif
 
+#ifndef GDK_WINDOWING_RISCOS
 #include <epoxy/gl.h>
+#endif
 #include <math.h>
 #include <string.h>
 
@@ -43,6 +45,7 @@
 static const char *
 get_vertex_type_name (int type)
 {
+#ifndef GDK_WINDOWING_RISCOS
   switch (type)
     {
     case GL_VERTEX_SHADER:
@@ -52,9 +55,11 @@
     case GL_FRAGMENT_SHADER:
       return "fragment";
     }
+#endif
   return "unknown";
 }
 
+#ifndef GDK_WINDOWING_RISCOS
 static guint
 create_shader (int         type,
                const char *code)
@@ -212,6 +217,7 @@
       glUseProgram (paint_data->current_program->program);
     }
 }
+#endif
 
 void
 gdk_gl_texture_quads (GdkGLContext *paint_context,
@@ -220,6 +226,7 @@
                       GdkTexturedQuad *quads,
                       gboolean flip_colors)
 {
+#ifndef GDK_WINDOWING_RISCOS
   GdkGLContextPaintData *paint_data  = gdk_gl_context_get_paint_data (paint_context);
   GdkGLContextProgram *program;
   GdkWindow *window = gdk_gl_context_get_window (paint_context);
@@ -293,6 +300,7 @@
 
   glDisableVertexAttribArray (program->position_location);
   glDisableVertexAttribArray (program->uv_location);
+#endif
 }
 
 /* x,y,width,height describes a rectangle in the gl render buffer
@@ -341,6 +349,7 @@
                         int                   width,
                         int                   height)
 {
+#ifndef GDK_WINDOWING_RISCOS
   GdkGLContext *paint_context;
   cairo_surface_t *image;
   cairo_matrix_t matrix;
@@ -717,7 +726,7 @@
 out:
   if (clip_region)
     cairo_region_destroy (clip_region);
-
+#endif
 }
 
 /* This is always called with the paint context current */
@@ -725,6 +734,7 @@
 gdk_gl_texture_from_surface (cairo_surface_t *surface,
 			     cairo_region_t  *region)
 {
+#ifndef GDK_WINDOWING_RISCOS
   GdkGLContext *paint_context;
   cairo_surface_t *image;
   double device_x_offset, device_y_offset;
@@ -825,4 +835,5 @@
 
   glDisable (GL_SCISSOR_TEST);
   glDeleteTextures (1, &texture_id);
+#endif
 }
