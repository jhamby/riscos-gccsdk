--- X/DispKbd.c.orig	2011-01-23 16:15:23.000000000 +0000
+++ X/DispKbd.c	2011-01-23 16:04:18.000000000 +0000
@@ -124,8 +124,8 @@
     const char *name;
     KeySym keysym;
 } mouse_key = {
-    "KP_Add",
-    XK_KP_Add
+    "F4",
+    XK_F4
 };

 /* Structure to hold most of the X windows values, shared with ControlPane.c */
