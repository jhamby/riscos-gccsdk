--- gcc/dwarf2out.c.orig	2006-09-24 23:03:43.000000000 +0200
+++ gcc/dwarf2out.c	2006-09-24 23:08:22.000000000 +0200
@@ -1524,9 +1524,9 @@
 	   cfa.base_offset = -cfa_temp.offset
 	   cfa_temp.offset -= mode_size(mem)
 
-  Rule 15:
-  (set <reg> {unspec, unspec_volatile})
-  effects: target-dependent  */
+ Rule 15:
+ (set <reg> {unspec, unspec_volatile})
+ effects: target-dependent  */
 
 static void
 dwarf2out_frame_debug_expr (rtx expr, const char *label)
@@ -1589,10 +1589,13 @@
 	  else
 	    {
 	      /* Saving a register in a register.  */
+	      /* NAB HACK (JOTY note: the assert condition got changed after NAB's hack so it might no longer be needed) */
+#if 0
 	      gcc_assert (!fixed_regs [REGNO (dest)]
 			  /* For the SPARC and its register window.  */
 			  || (DWARF_FRAME_REGNUM (REGNO (src))
 			      == DWARF_FRAME_RETURN_COLUMN));
+#endif
 	      queue_reg_save (label, src, dest, 0);
 	    }
 	  break;
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
