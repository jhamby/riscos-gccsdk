--- libgnomevfs/gnome-vfs-private-utils.c.orig	2009-08-01 10:59:46.000000000 -0700
+++ libgnomevfs/gnome-vfs-private-utils.c	2009-08-01 11:01:15.000000000 -0700
@@ -344,7 +344,7 @@
 gnome_vfs_atotm (const gchar *time_string,
 		 time_t *value_return)
 {
-#ifndef G_OS_WIN32
+#if !defined(G_OS_WIN32) && !defined(__riscos__)
 	struct tm t;
 
 	/* Roger Beeman says: "This function dynamically allocates struct tm
