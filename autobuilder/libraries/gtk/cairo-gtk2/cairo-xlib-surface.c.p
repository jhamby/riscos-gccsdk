--- src/cairo-xlib-surface.c.orig	2021-08-03 22:14:48.866995554 -0700
+++ src/cairo-xlib-surface.c	2021-08-03 22:15:16.659189847 -0700
@@ -68,9 +68,11 @@
 
 #include <X11/Xutil.h> /* for XDestroyImage */
 
+#ifndef __riscos__
 #include <X11/extensions/XShm.h>
 #include <sys/ipc.h>
 #include <sys/shm.h>
+#endif
 
 #define XLIB_COORD_MAX 32767
 
@@ -776,6 +778,7 @@
         return _cairo_surface_create_in_error (status);
 
     pixman_format = _pixman_format_for_xlib_surface (surface);
+#ifndef __riscos__
     if (try_shm && pixman_format) {
 	image = (cairo_image_surface_t *)
 	    _cairo_xlib_surface_create_shm__image (surface, pixman_format,
@@ -805,6 +808,7 @@
 	    image = NULL;
 	}
     }
+#endif
 
     if (surface->use_pixmap == 0) {
 	cairo_xlib_error_func_t old_handler;
@@ -1342,10 +1346,12 @@
     if (unlikely (status))
 	goto BAIL;
 
+#ifndef __riscos__
     if (ximage.obdata)
 	XShmPutImage (display->display, surface->drawable, gc, &ximage,
 		      src_x, src_y, dst_x, dst_y, width, height, True);
     else
+#endif
 	XPutImage (display->display, surface->drawable, gc, &ximage,
 		   src_x, src_y, dst_x, dst_y, width, height);
 
