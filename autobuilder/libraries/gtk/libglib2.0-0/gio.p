--- gio/gunixmounts.c.orig	2017-04-01 20:02:42.456064630 +0100
+++ gio/gunixmounts.c	2017-04-01 20:09:09.876069650 +0100
@@ -828,6 +828,20 @@
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
@@ -1348,6 +1362,14 @@
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
