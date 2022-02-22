--- gdk/gdkdisplaymanager.c.orig	2020-02-07 00:03:55.000000000 +0000
+++ gdk/gdkdisplaymanager.c	2020-07-29 18:08:42.962141942 +0100
@@ -54,6 +54,11 @@
 #include "wayland/gdkprivate-wayland.h"
 #endif
 
+#ifdef GDK_WINDOWING_RISCOS
+#include "riscos/gdkriscos.h"
+#include "riscos/gdkprivate-riscos.h"
+#endif
+
 /**
  * SECTION:gdkdisplaymanager
  * @Short_description: Maintains a list of all open GdkDisplays
@@ -272,6 +277,9 @@
 #ifdef GDK_WINDOWING_BROADWAY
   { "broadway", _gdk_broadway_display_open },
 #endif
+#ifdef GDK_WINDOWING_RISCOS
+  { "riscos", _gdk_riscos_display_open },
+#endif
   /* NULL-terminating this array so we can use commas above */
   { NULL, NULL }
 };
