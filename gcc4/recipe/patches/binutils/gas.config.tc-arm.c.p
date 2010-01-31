--- gas/config/tc-arm.c.orig	2008-12-10 00:30:14.000000000 +0100
+++ gas/config/tc-arm.c	2008-12-10 00:30:02.000000000 +0100
@@ -2469,7 +2469,7 @@ s_arm_unwind_save_mmxwr (void)
 
       if (reg == FAIL)
 	{
-	  as_bad (_(reg_expected_msgs[REG_TYPE_MMXWR]));
+	  as_bad ("%s", reg_expected_msgs[REG_TYPE_MMXWR]);
 	  goto error;
 	}
 
@@ -2483,7 +2483,7 @@ s_arm_unwind_save_mmxwr (void)
 	  hi_reg = arm_reg_parse (&input_line_pointer, REG_TYPE_MMXWR);
 	  if (hi_reg == FAIL)
 	    {
-	      as_bad (_(reg_expected_msgs[REG_TYPE_MMXWR]));
+	      as_bad ("%s", reg_expected_msgs[REG_TYPE_MMXWR]);
 	      goto error;
 	    }
 	  else if (reg >= hi_reg)
@@ -2601,7 +2601,7 @@ s_arm_unwind_save_mmxwcg (void)
 
       if (reg == FAIL)
 	{
-	  as_bad (_(reg_expected_msgs[REG_TYPE_MMXWCG]));
+	  as_bad ("%s", reg_expected_msgs[REG_TYPE_MMXWCG]);
 	  goto error;
 	}
 
@@ -2616,7 +2616,7 @@ s_arm_unwind_save_mmxwcg (void)
 	  hi_reg = arm_reg_parse (&input_line_pointer, REG_TYPE_MMXWCG);
 	  if (hi_reg == FAIL)
 	    {
-	      as_bad (_(reg_expected_msgs[REG_TYPE_MMXWCG]));
+	      as_bad ("%s", reg_expected_msgs[REG_TYPE_MMXWCG]);
 	      goto error;
 	    }
 	  else if (reg >= hi_reg)
@@ -2714,7 +2714,7 @@ s_arm_unwind_movsp (int ignored ATTRIBUT
   reg = arm_reg_parse (&input_line_pointer, REG_TYPE_RN);
   if (reg == FAIL)
     {
-      as_bad (_(reg_expected_msgs[REG_TYPE_RN]));
+      as_bad ("%s", reg_expected_msgs[REG_TYPE_RN]);
       ignore_rest_of_line ();
       return;
     }
@@ -3842,7 +3842,7 @@ parse_operands (char *str, const unsigne
   unsigned const char *upat = pattern;
   char *backtrack_pos = 0;
   const char *backtrack_error = 0;
-  int i, val, backtrack_index = 0;
+  int i, val = 0, backtrack_index = 0;
 
 #define po_char_or_fail(chr) do {		\
   if (skip_past_char (&str, chr) == FAIL)	\
