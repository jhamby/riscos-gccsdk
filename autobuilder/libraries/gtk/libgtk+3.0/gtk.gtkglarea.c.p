--- gtk/gtkglarea.c.orig	2020-06-11 03:45:39.000000000 +0100
+++ gtk/gtkglarea.c	2020-11-22 21:54:44.887462567 +0000
@@ -29,7 +29,9 @@
 #include "gtkprivate.h"
 #include "gtkrender.h"
 
+#ifndef GDK_WINDOWING_RISCOS
 #include <epoxy/gl.h>
+#endif
 
 /**
  * SECTION:gtkglarea
@@ -369,9 +371,12 @@
 static void
 gtk_gl_area_resize (GtkGLArea *area, int width, int height)
 {
+#ifndef GDK_WINDOWING_RISCOS
   glViewport (0, 0, width, height);
+#endif
 }
 
+#ifndef GDK_WINDOWING_RISCOS
 /*
  * Creates all the buffer objects needed for rendering the scene
  */
@@ -483,6 +488,7 @@
 
   priv->needs_render = TRUE;
 }
+#endif
 
 /**
  * gtk_gl_area_attach_buffers:
@@ -501,6 +507,7 @@
 void
 gtk_gl_area_attach_buffers (GtkGLArea *area)
 {
+#ifndef GDK_WINDOWING_RISCOS
   GtkGLAreaPrivate *priv = gtk_gl_area_get_instance_private (area);
 
   g_return_if_fail (GTK_IS_GL_AREA (area));
@@ -533,6 +540,7 @@
         glFramebufferRenderbufferEXT (GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT,
                                       GL_RENDERBUFFER_EXT, priv->depth_stencil_buffer);
     }
+#endif
 }
 
 static void
@@ -545,6 +553,7 @@
 
   priv->have_buffers = FALSE;
 
+#ifndef GDK_WINDOWING_RISCOS
   if (priv->render_buffer != 0)
     {
       glDeleteRenderbuffersEXT (1, &priv->render_buffer);
@@ -569,11 +578,13 @@
       glDeleteFramebuffersEXT (1, &priv->frame_buffer);
       priv->frame_buffer = 0;
     }
+#endif
 }
 
 static void
 gtk_gl_area_unrealize (GtkWidget *widget)
 {
+#ifndef GDK_WINDOWING_RISCOS
   GtkGLArea *area = GTK_GL_AREA (widget);
   GtkGLAreaPrivate *priv = gtk_gl_area_get_instance_private (area);
 
@@ -601,6 +612,7 @@
     }
 
   GTK_WIDGET_CLASS (gtk_gl_area_parent_class)->unrealize (widget);
+#endif
 }
 
 static void
@@ -676,6 +688,9 @@
 gtk_gl_area_draw (GtkWidget *widget,
                   cairo_t   *cr)
 {
+#ifdef GDK_WINDOWING_RISCOS
+  return FALSE;
+#else
   GtkGLArea *area = GTK_GL_AREA (widget);
   GtkGLAreaPrivate *priv = gtk_gl_area_get_instance_private (area);
   gboolean unused;
@@ -736,6 +751,7 @@
     }
 
   return TRUE;
+#endif
 }
 
 static gboolean
