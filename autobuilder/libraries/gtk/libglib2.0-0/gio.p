--- gio/gunixmounts.c.orig	2008-04-21 04:48:52.000000000 -0700
+++ gio/gunixmounts.c	2008-04-21 04:51:47.000000000 -0700
@@ -599,6 +599,20 @@
   
   return g_list_reverse (return_list);
 }
+#elif defined(__riscos__)
+
+static char *
+get_mtab_monitor_file (void)
+{
+  return NULL;
+}
+
+static GList *
+_g_get_unix_mounts (void)
+{
+  return NULL;
+}
+
 #else
 #error No _g_get_unix_mounts() implementation for system
 #endif
@@ -979,6 +993,15 @@
   
   return g_list_reverse (return_list);
 }
+
+#elif defined(__riscos__)
+
+static GList *
+_g_get_unix_mount_points (void)
+{
+  return NULL;
+}
+
 #else
 #error No g_get_mount_table() implementation for system
 #endif
