--- gcc/dwarf2out.c.orig	2016-09-01 12:43:15.000000000 +0100
+++ gcc/dwarf2out.c	2017-02-06 17:31:16.940341879 +0000
@@ -18043,12 +18043,16 @@
      this, assume that while we cannot provide a proper value for
      frame_pointer_fb_offset, we won't need one either.  */
   frame_pointer_fb_offset_valid
+#if 1	/* FIXME: GROSS HACK ? */
+    = true;
+#else
     = ((SUPPORTS_STACK_ALIGNMENT
 	&& (elim == hard_frame_pointer_rtx
 	    || elim == stack_pointer_rtx))
        || elim == (frame_pointer_needed
 		   ? hard_frame_pointer_rtx
 		   : stack_pointer_rtx));
+#endif
 }
 
 /* Generate a DW_AT_name attribute given some string value to be included as
