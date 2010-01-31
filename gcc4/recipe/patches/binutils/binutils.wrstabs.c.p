--- binutils/wrstabs.c.orig	2007-10-09 22:25:19.000000000 +0200
+++ binutils/wrstabs.c	2007-10-09 22:25:06.000000000 +0200
@@ -1869,7 +1869,7 @@
 {
   struct stab_write_handle *info = (struct stab_write_handle *) p;
   long index;
-  unsigned int size;
+  unsigned int size = 0;
 
   index = stab_get_struct_index (info, name, id, kind, &size);
   if (index < 0)
