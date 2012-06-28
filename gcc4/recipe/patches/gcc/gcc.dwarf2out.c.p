Index: gcc/dwarf2out.c
===================================================================
--- gcc/dwarf2out.c	(revision 189025)
+++ gcc/dwarf2out.c	(working copy)
@@ -15456,12 +15456,16 @@
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
