--- gdk/gdk.c.orig	2013-10-17 00:28:05.000000000 +0100
+++ gdk/gdk.c	2019-10-27 14:06:51.223287331 +0000
@@ -28,6 +28,10 @@
 
 #include "gdkmain.h"
 
+#ifdef GDK_WINDOWING_RISCOS
+#include "riscos/gdkprivate-riscos.h"
+#endif
+
 #include "gdkinternals.h"
 #include "gdkintl.h"
 
@@ -200,6 +204,19 @@
   return TRUE;
 }
 
+#ifdef GDK_WINDOWING_RISCOS
+static gboolean
+gdk_arg_redblue_cb (const char *key, const char *value, gpointer user_data, GError **error)
+{
+  if (stricmp(value, "yes") == 0 || stricmp(value, "1"))
+    swap_redblue = swap_redblue_YES;
+  else if (stricmp(value, "no") == 0 || stricmp(value, "0"))
+    swap_redblue = swap_redblue_NO;
+
+  return TRUE;
+}
+#endif
+
 static const GOptionEntry gdk_args[] = {
   { "class",        0, 0,                     G_OPTION_ARG_CALLBACK, gdk_arg_class_cb,
     /* Description of --class=CLASS in --help output */        N_("Program class as used by the window manager"),
@@ -210,6 +227,11 @@
   { "display",      0, G_OPTION_FLAG_IN_MAIN, G_OPTION_ARG_STRING,   &_gdk_display_name,
     /* Description of --display=DISPLAY in --help output */    N_("X display to use"),
     /* Placeholder in --display=DISPLAY in --help output */    N_("DISPLAY") },
+#ifdef GDK_WINDOWING_RISCOS
+  { "swap-redblue", 0, 0,                     G_OPTION_ARG_CALLBACK, gdk_arg_redblue_cb,
+    /* Description of --swap-redblue=VALUE in --help output */ N_("1 if swapping of red and blue components required"),
+    /* Placeholder in --swap-redblue=VALUE in --help output */ N_("VALUE") },
+#endif
 #ifdef G_ENABLE_DEBUG
   { "gdk-debug",    0, 0, G_OPTION_ARG_CALLBACK, gdk_arg_debug_cb,  
     /* Description of --gdk-debug=FLAGS in --help output */    N_("GDK debugging flags to set"),
