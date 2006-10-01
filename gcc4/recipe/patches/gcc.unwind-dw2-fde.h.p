Index: gcc/unwind-dw2-fde.h
===================================================================
RCS file: gcc/unwind-dw2-fde.h,v
retrieving revision 1.16
diff -u -r1.16 unwind-dw2-fde.h
--- gcc/unwind-dw2-fde.h	30 Nov 2004 08:15:39 -0000	1.16
+++ gcc/unwind-dw2-fde.h	17 May 2006 21:59:02 -0000
@@ -177,7 +177,7 @@
 #ifdef DWARF2_OBJECT_END_PTR_EXTENSION
   return (char *)f == obj->fde_end || f->length == 0;
 #else
-  return f->length == 0;
+  return f->length == 0 || f->length == 0xffffffff;
 #endif
 }
 
