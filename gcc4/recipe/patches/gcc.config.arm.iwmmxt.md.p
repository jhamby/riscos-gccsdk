Index: gcc/config/arm/iwmmxt.md
===================================================================
RCS file: gcc/config/arm/iwmmxt.md,v
retrieving revision 1.9
diff -u -r1.9 iwmmxt.md
--- gcc/config/arm/iwmmxt.md	7 Apr 2005 11:24:10 -0000	1.9
+++ gcc/config/arm/iwmmxt.md	17 May 2006 21:59:32 -0000
@@ -222,7 +222,7 @@
 ;; deliberately omitted.
 (define_insn "movv2si_internal_2"
   [(set (match_operand:V2SI 0 "nonimmediate_operand" "=?r")
-	(match_operand      1 "immediate_operand"      "mi"))]
+	(match_operand:V2SI 1 "immediate_operand"      "mi"))]
   "TARGET_REALLY_IWMMXT"
   "* return output_move_double (operands);"
   [(set_attr "predicable"     "yes")
