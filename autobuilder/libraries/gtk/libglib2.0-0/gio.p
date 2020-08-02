--- gio/gunixmounts.c.orig	2019-01-21 13:38:50.000000000 +0000
+++ gio/gunixmounts.c	2020-08-02 17:55:12.113956435 +0100
@@ -934,6 +934,20 @@
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
 /* Common code {{{2 */
 #else
 #error No _g_get_unix_mounts() implementation for system
@@ -1457,6 +1471,14 @@
   return _g_get_unix_mounts ();
 }
 
+#elif defined(__riscos__)
+
+static GList *
+_g_get_unix_mount_points (void)
+{
+  return NULL;
+}
+
 /* Common code {{{2 */
 #else
 #error No g_get_mount_table() implementation for system
