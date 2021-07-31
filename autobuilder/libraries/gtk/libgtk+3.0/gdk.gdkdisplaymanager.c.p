--- gdk/gdkdisplaymanager.c.orig	2013-10-17 00:28:05.000000000 +0100
+++ gdk/gdkdisplaymanager.c	2014-05-11 20:22:39.081277636 +0100
@@ -55,6 +55,10 @@
 #include "wayland/gdkwayland.h"
 #endif
 
+#ifdef GDK_WINDOWING_RISCOS
+#include "riscos/gdkriscos.h"
+#endif
+
 /**
  * SECTION:gdkdisplaymanager
  * @Short_description: Maintains a list of all open GdkDisplays
@@ -259,6 +263,11 @@
         manager = g_object_new (gdk_broadway_display_manager_get_type (), NULL);
       else
 #endif
+#ifdef GDK_WINDOWING_RISCOS
+      if (backend == NULL || strcmp (backend, "riscos") == 0)
+	manager = g_object_new (gdk_riscos_display_manager_get_type (), NULL);
+      else
+#endif
       if (backend != NULL)
         g_error ("Unsupported GDK backend: %s", backend);
       else
