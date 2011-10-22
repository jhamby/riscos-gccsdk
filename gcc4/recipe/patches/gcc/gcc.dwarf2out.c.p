Index: gcc/dwarf2out.c
===================================================================
--- gcc/dwarf2out.c	(revision 180325)
+++ gcc/dwarf2out.c	(working copy)
@@ -2446,7 +2446,9 @@
 	      /* Rule 3 */
 	      /* Either setting the FP from an offset of the SP,
 		 or adjusting the FP */
+#ifndef TARGET_RISCOSELF
 	      gcc_assert (frame_pointer_needed);
+#endif
 
 	      gcc_assert (REG_P (XEXP (src, 0))
 			  && (unsigned) REGNO (XEXP (src, 0)) == cfa.reg
@@ -2495,6 +2497,13 @@
 		  cfa_temp.reg = REGNO (dest);
 		  cfa_temp.offset = INTVAL (XEXP (src, 1));
 		}
+	      /* Rule XX */
+	      else if (GET_CODE (src) == PLUS
+		       && GET_CODE (XEXP (src, 1)) == CONST_INT)
+		{
+		  cfa_temp.reg = REGNO (dest);
+		  cfa_temp.offset = INTVAL (XEXP (src, 1));
+		}
 	      else
 		gcc_unreachable ();
 	    }
@@ -17418,12 +17427,16 @@
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
