--- gdk/gdkglcontext.c.orig	2020-06-11 03:45:39.000000000 +0100
+++ gdk/gdkglcontext.c	2020-11-22 22:03:09.042083627 +0000
@@ -85,7 +85,9 @@
 #include "gdkintl.h"
 #include "gdk-private.h"
 
+#ifndef GDK_WINDOWING_RISCOS
 #include <epoxy/gl.h>
+#endif
 
 typedef struct {
   GdkDisplay *display;
@@ -243,6 +245,7 @@
                                int              height,
                                guint            texture_target)
 {
+#ifndef GDK_WINDOWING_RISCOS
   GdkGLContextPrivate *priv = gdk_gl_context_get_instance_private (context);
 
   g_return_if_fail (GDK_IS_GL_CONTEXT (context));
@@ -286,6 +289,7 @@
             glTexSubImage2D (texture_target, 0, 0, i, width, 1, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, (unsigned char*) data + (i * stride));
         }
     }
+#endif
 }
 
 static gboolean
@@ -388,15 +392,19 @@
                           cairo_region_t *painted,
                           cairo_region_t *damage)
 {
+#ifndef GDK_WINDOWING_RISCOS
   g_return_if_fail (GDK_IS_GL_CONTEXT (context));
 
   GDK_GL_CONTEXT_GET_CLASS (context)->end_frame (context, painted, damage);
+#endif
 }
 
 GdkGLContextPaintData *
 gdk_gl_context_get_paint_data (GdkGLContext *context)
 {
-
+#ifdef GDK_WINDOWING_RISCOS
+  return NULL;
+#else
   GdkGLContextPrivate *priv = gdk_gl_context_get_instance_private (context);
 
   if (priv->paint_data == NULL)
@@ -407,38 +415,55 @@
     }
 
   return priv->paint_data;
+#endif
 }
 
 gboolean
 gdk_gl_context_use_texture_rectangle (GdkGLContext *context)
 {
+#ifdef GDK_WINDOWING_RISCOS
+  return FALSE;
+#else
   GdkGLContextPrivate *priv = gdk_gl_context_get_instance_private (context);
 
   return priv->use_texture_rectangle;
+#endif
 }
 
 gboolean
 gdk_gl_context_has_framebuffer_blit (GdkGLContext *context)
 {
+#ifdef GDK_WINDOWING_RISCOS
+  return FALSE;
+#else
   GdkGLContextPrivate *priv = gdk_gl_context_get_instance_private (context);
 
   return priv->has_gl_framebuffer_blit;
+#endif
 }
 
 gboolean
 gdk_gl_context_has_frame_terminator (GdkGLContext *context)
 {
+#ifdef GDK_WINDOWING_RISCOS
+  return FALSE;
+#else
   GdkGLContextPrivate *priv = gdk_gl_context_get_instance_private (context);
 
   return priv->has_frame_terminator;
+#endif
 }
 
 gboolean
 gdk_gl_context_has_unpack_subimage (GdkGLContext *context)
 {
+#ifdef GDK_WINDOWING_RISCOS
+  return FALSE;
+#else
   GdkGLContextPrivate *priv = gdk_gl_context_get_instance_private (context);
 
   return priv->has_unpack_subimage;
+#endif
 }
 
 /**
@@ -459,6 +484,7 @@
 gdk_gl_context_set_debug_enabled (GdkGLContext *context,
                                   gboolean      enabled)
 {
+#ifndef GDK_WINDOWING_RISCOS
   GdkGLContextPrivate *priv = gdk_gl_context_get_instance_private (context);
 
   g_return_if_fail (GDK_IS_GL_CONTEXT (context));
@@ -467,6 +493,7 @@
   enabled = !!enabled;
 
   priv->debug_enabled = enabled;
+#endif
 }
 
 /**
@@ -482,11 +509,15 @@
 gboolean
 gdk_gl_context_get_debug_enabled (GdkGLContext *context)
 {
+#ifdef GDK_WINDOWING_RISCOS
+  return FALSE;
+#else
   GdkGLContextPrivate *priv = gdk_gl_context_get_instance_private (context);
 
   g_return_val_if_fail (GDK_IS_GL_CONTEXT (context), FALSE);
 
   return priv->debug_enabled;
+#endif
 }
 
 /**
@@ -510,6 +541,7 @@
 gdk_gl_context_set_forward_compatible (GdkGLContext *context,
                                        gboolean      compatible)
 {
+#ifndef GDK_WINDOWING_RISCOS
   GdkGLContextPrivate *priv = gdk_gl_context_get_instance_private (context);
 
   g_return_if_fail (GDK_IS_GL_CONTEXT (context));
@@ -518,6 +550,7 @@
   compatible = !!compatible;
 
   priv->forward_compatible = compatible;
+#endif
 }
 
 /**
@@ -533,11 +566,15 @@
 gboolean
 gdk_gl_context_get_forward_compatible (GdkGLContext *context)
 {
+#ifdef GDK_WINDOWING_RISCOS
+  return FALSE;
+#else
   GdkGLContextPrivate *priv = gdk_gl_context_get_instance_private (context);
 
   g_return_val_if_fail (GDK_IS_GL_CONTEXT (context), FALSE);
 
   return priv->forward_compatible;
+#endif
 }
 
 /**
@@ -560,6 +597,7 @@
                                      int           major,
                                      int           minor)
 {
+#ifndef GDK_WINDOWING_RISCOS
   GdkGLContextPrivate *priv = gdk_gl_context_get_instance_private (context);
   int version, min_ver;
 
@@ -589,6 +627,7 @@
     }
   priv->major = version / 100;
   priv->minor = version % 100;
+#endif
 }
 
 /**
@@ -607,6 +646,7 @@
                                      int          *major,
                                      int          *minor)
 {
+#ifndef GDK_WINDOWING_RISCOS
   GdkGLContextPrivate *priv = gdk_gl_context_get_instance_private (context);
   int default_major, default_minor;
   int maj, min;
@@ -638,6 +678,7 @@
     *major = maj;
   if (minor != NULL)
     *minor = min;
+#endif
 }
 
 /**
@@ -668,21 +709,27 @@
 gboolean
 gdk_gl_context_is_legacy (GdkGLContext *context)
 {
+#ifdef GDK_WINDOWING_RISCOS
+  return TRUE;
+#else
   GdkGLContextPrivate *priv = gdk_gl_context_get_instance_private (context);
 
   g_return_val_if_fail (GDK_IS_GL_CONTEXT (context), FALSE);
   g_return_val_if_fail (priv->realized, FALSE);
 
   return priv->is_legacy;
+#endif
 }
 
 void
 gdk_gl_context_set_is_legacy (GdkGLContext *context,
                               gboolean      is_legacy)
 {
+#ifndef GDK_WINDOWING_RISCOS
   GdkGLContextPrivate *priv = gdk_gl_context_get_instance_private (context);
 
   priv->is_legacy = !!is_legacy;
+#endif
 }
 
 /**
@@ -710,6 +757,7 @@
 gdk_gl_context_set_use_es (GdkGLContext *context,
                            int           use_es)
 {
+#ifndef GDK_WINDOWING_RISCOS
   GdkGLContextPrivate *priv = gdk_gl_context_get_instance_private (context);
 
   g_return_if_fail (GDK_IS_GL_CONTEXT (context));
@@ -717,6 +765,7 @@
 
   if (priv->use_es != use_es)
     priv->use_es = use_es;
+#endif
 }
 
 /**
@@ -732,6 +781,9 @@
 gboolean
 gdk_gl_context_get_use_es (GdkGLContext *context)
 {
+#ifdef GDK_WINDOWING_RISCOS
+  return FALSE;
+#else
   GdkGLContextPrivate *priv = gdk_gl_context_get_instance_private (context);
 
   g_return_val_if_fail (GDK_IS_GL_CONTEXT (context), FALSE);
@@ -740,6 +792,7 @@
     return FALSE;
 
   return priv->use_es > 0;
+#endif
 }
 
 /**
@@ -759,6 +812,9 @@
 gdk_gl_context_realize (GdkGLContext  *context,
                         GError       **error)
 {
+#ifdef GDK_WINDOWING_RISCOS
+  return FALSE;
+#else
   GdkGLContextPrivate *priv = gdk_gl_context_get_instance_private (context);
 
   g_return_val_if_fail (GDK_IS_GL_CONTEXT (context), FALSE);
@@ -769,11 +825,13 @@
   priv->realized = GDK_GL_CONTEXT_GET_CLASS (context)->realize (context, error);
 
   return priv->realized;
+#endif
 }
 
 static void
 gdk_gl_context_check_extensions (GdkGLContext *context)
 {
+#ifndef GDK_WINDOWING_RISCOS
   GdkGLContextPrivate *priv = gdk_gl_context_get_instance_private (context);
   gboolean has_npot, has_texture_rectangle;
 
@@ -853,6 +911,7 @@
                        priv->use_texture_rectangle ? "yes" : "no"));
 
   priv->extensions_checked = TRUE;
+#endif
 }
 
 /**
@@ -866,6 +925,7 @@
 void
 gdk_gl_context_make_current (GdkGLContext *context)
 {
+#ifndef GDK_WINDOWING_RISCOS
   GdkGLContextPrivate *priv = gdk_gl_context_get_instance_private (context);
   GdkGLContext *current;
 
@@ -894,6 +954,7 @@
       g_private_replace (&thread_current_context, g_object_ref (context));
       gdk_gl_context_check_extensions (context);
     }
+#endif
 }
 
 /**
