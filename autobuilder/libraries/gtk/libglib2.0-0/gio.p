--- gio/gunixmounts.c.orig	2021-03-18 13:47:48.292693100 +0000
+++ gio/gunixmounts.c	2022-01-14 11:46:15.220532194 +0000
@@ -956,6 +956,20 @@
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
@@ -1479,6 +1493,14 @@
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
