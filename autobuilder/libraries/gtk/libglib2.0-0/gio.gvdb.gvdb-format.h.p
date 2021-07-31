--- gio/gvdb/gvdb-format.h.orig	2014-02-03 17:40:41.000000000 +0000
+++ gio/gvdb/gvdb-format.h	2014-05-08 17:08:34.116137594 +0100
@@ -22,7 +22,7 @@
 
 #include <glib.h>
 
-typedef struct { guint16 value; } guint16_le;
+typedef struct { guint16 value; }  __attribute__((packed)) guint16_le;
 typedef struct { guint32 value; } guint32_le;
 
 struct gvdb_pointer {
