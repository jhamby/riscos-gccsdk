--- gcc/dwarf2out.c.orig	2023-07-30 13:13:45.363459238 -0700
+++ gcc/dwarf2out.c	2023-07-30 13:28:37.920497166 -0700
@@ -20493,7 +20493,11 @@
      which uses the DW_AT_frame_base attribute, not hard frame pointer
      directly.  */
   frame_pointer_fb_offset_valid
+#if 1	/* FIXME: GROSS HACK ? */
+    = true;
+#else
     = (elim == hard_frame_pointer_rtx || elim == stack_pointer_rtx);
+#endif
 }
 
 /* Generate a DW_AT_name attribute given some string value to be included as
