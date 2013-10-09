--- gio/gvdb/gvdb-format.h.orig	2013-10-09 12:13:29.000000000 +0100
+++ gio/gvdb/gvdb-format.h	2013-10-09 12:13:49.173950480 +0100
@@ -24,7 +24,7 @@
 
 #include <glib.h>
 
-typedef struct { guint16 value; } guint16_le;
+typedef struct { guint16 value; }  __attribute__((packed)) guint16_le;
 typedef struct { guint32 value; } guint32_le;
 
 struct gvdb_pointer {
