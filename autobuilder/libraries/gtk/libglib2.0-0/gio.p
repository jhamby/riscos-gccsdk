--- gio/gunixmounts.c.orig	2013-08-07 15:34:32.000000000 +0100
+++ gio/gunixmounts.c	2013-10-09 12:21:28.173944500 +0100
@@ -716,6 +716,20 @@
 
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
@@ -1110,6 +1124,15 @@
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
