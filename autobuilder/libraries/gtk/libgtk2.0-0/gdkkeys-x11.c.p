--- gdk/x11/gdkkeys-x11.c.orig	2011-12-29 17:51:15.000000000 +0000
+++ gdk/x11/gdkkeys-x11.c	2011-12-29 17:51:54.000000000 +0000
@@ -1816,8 +1816,10 @@
   keymap = GET_EFFECTIVE_KEYMAP (keymap);
   keymap_x11 = GDK_KEYMAP_X11 (keymap);
 
+#ifdef HAVE_XKB
   if (KEYMAP_USE_XKB (keymap))
     get_xkb (keymap_x11);
+#endif
 
   retval = TRUE;
 
