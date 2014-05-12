--- gio/gunixmounts.c.orig	2014-02-12 20:08:36.000000000 +0000
+++ gio/gunixmounts.c	2014-05-08 17:08:34.092137593 +0100
@@ -722,6 +722,20 @@
 
   return return_list;
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
@@ -1118,6 +1132,15 @@
 {
   return _g_get_unix_mounts ();
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
