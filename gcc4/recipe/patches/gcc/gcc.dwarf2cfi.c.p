Index: gcc/dwarf2cfi.c
===================================================================
--- gcc/dwarf2cfi.c	(revision 177297)
+++ gcc/dwarf2cfi.c	(working copy)
@@ -1598,7 +1598,9 @@
 	      /* Rule 3 */
 	      /* Either setting the FP from an offset of the SP,
 		 or adjusting the FP */
+#ifndef TARGET_RISCOSELF
 	      gcc_assert (frame_pointer_needed);
+#endif
 
 	      gcc_assert (REG_P (XEXP (src, 0))
 			  && dwf_regno (XEXP (src, 0)) == cur_cfa->reg
@@ -1648,6 +1650,14 @@
 		  cur_trace->cfa_temp.reg = dwf_regno (dest);
 		  cur_trace->cfa_temp.offset = INTVAL (XEXP (src, 1));
 		}
+
+	      /* Rule XX */
+	      else if (GET_CODE (src) == PLUS
+		       && GET_CODE (XEXP (src, 1)) == CONST_INT)
+		{
+		  cur_trace->cfa_temp.reg = REGNO (dest);
+		  cur_trace->cfa_temp.offset = INTVAL (XEXP (src, 1));
+		}
 	      else
 		gcc_unreachable ();
 	    }
