--- gcc/dwarf2cfi.c.orig	2016-01-04 14:30:50.000000000 +0000
+++ gcc/dwarf2cfi.c	2016-10-25 17:44:24.385363490 +0100
@@ -1655,7 +1655,9 @@
 	      /* Rule 3 */
 	      /* Either setting the FP from an offset of the SP,
 		 or adjusting the FP */
+#ifndef TARGET_RISCOSELF
 	      gcc_assert (frame_pointer_needed);
+#endif
 
 	      gcc_assert (REG_P (XEXP (src, 0))
 			  && dwf_regno (XEXP (src, 0)) == cur_cfa->reg
@@ -1705,6 +1707,14 @@
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
