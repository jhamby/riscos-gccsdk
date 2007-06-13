--- gcc/dwarf2out.c.orig	2006-09-24 23:03:43.000000000 +0200
+++ gcc/dwarf2out.c	2006-09-24 23:08:22.000000000 +0200
@@ -1691,6 +1694,13 @@
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
