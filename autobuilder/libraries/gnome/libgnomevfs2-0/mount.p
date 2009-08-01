--- libgnomevfs/gnome-vfs-unix-mounts.c.orig	2009-08-01 10:53:34.000000000 -0700
+++ libgnomevfs/gnome-vfs-unix-mounts.c	2009-08-01 11:09:01.000000000 -0700
@@ -515,7 +515,6 @@
 
 	return TRUE;
 }
-#else
 #error No _gnome_vfs_get_current_unix_mounts() implementation for system
 #endif
 
@@ -996,6 +995,26 @@
 
 	return TRUE;
 }
+#elif defined(__riscos__)
+
+gboolean
+_gnome_vfs_get_current_unix_mounts (GList **return_list)
+{
+        *return_list = NULL;
+        return TRUE;
+}
+
+static char *get_mtab_monitor_file (void)
+{
+	return NULL;
+}
+
+gboolean
+_gnome_vfs_get_unix_mount_table (GList **return_list)
+{
+	*return_list = NULL;
+	return TRUE;
+}
 #else
 #error No _gnome_vfs_get_mount_table() implementation for system
 #endif
