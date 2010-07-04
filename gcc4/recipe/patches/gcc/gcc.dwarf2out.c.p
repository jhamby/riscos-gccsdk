--- gcc/dwarf2out.c.orig	2006-12-27 23:23:55.000000000 +0100
+++ gcc/dwarf2out.c	2010-07-04 19:34:09.884398568 +0200
@@ -1697,6 +1697,13 @@ dwarf2out_frame_debug_expr (rtx expr, co
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
