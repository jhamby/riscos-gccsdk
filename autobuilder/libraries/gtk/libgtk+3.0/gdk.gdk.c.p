--- gdk/gdk.c.orig	2020-08-26 19:45:22.000000000 +0100
+++ gdk/gdk.c	2020-08-26 19:45:40.700373825 +0100
@@ -27,6 +27,10 @@
 #include "gdkversionmacros.h"
 #include "gdkmain.h"
 
+#ifdef GDK_WINDOWING_RISCOS
+#include "riscos/gdkprivate-riscos.h"
+#endif
+
 #include "gdkprofilerprivate.h"
 #include "gdkinternals.h"
 #include "gdkintl.h"
@@ -223,6 +227,19 @@
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
@@ -235,6 +252,11 @@
     /* Description of --display=DISPLAY in --help output */    N_("X display to use"),
     /* Placeholder in --display=DISPLAY in --help output */    N_("DISPLAY") },
 #endif
+#ifdef GDK_WINDOWING_RISCOS
+  { "swap-redblue", 0, 0,                     G_OPTION_ARG_CALLBACK, gdk_arg_redblue_cb,
+    /* Description of --swap-redblue=VALUE in --help output */ N_("1 if swapping of red and blue components required"),
+    /* Placeholder in --swap-redblue=VALUE in --help output */ N_("VALUE") },
+#endif
 #ifdef G_ENABLE_DEBUG
   { "gdk-debug",    0, 0, G_OPTION_ARG_CALLBACK, gdk_arg_debug_cb,  
     /* Description of --gdk-debug=FLAGS in --help output */    N_("GDK debugging flags to set"),
