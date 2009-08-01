--- libgnomevfs/gnome-vfs-dns-sd.c.orig	2009-08-01 11:03:04.000000000 -0700
+++ libgnomevfs/gnome-vfs-dns-sd.c	2009-08-01 11:07:36.000000000 -0700
@@ -325,6 +325,11 @@
 	return p - start;
 }
 
+static int my_res_init() {
+  return 0;
+}
+#define res_init my_res_init
+
 static GnomeVFSResult
 unicast_list_domains_sync (const char *domain,
 			   GList **domains)
