--- gas/config/tc-arm.c.orig	2013-04-01 20:56:08.485014906 +0200
+++ gas/config/tc-arm.c	2013-04-01 20:38:14.530516343 +0200
@@ -885,6 +885,9 @@ const char FLT_CHARS[] = "rRsSfFdDxXeEpP
 static inline int
 skip_past_char (char ** str, char c)
 {
+  /* PR gas/14987: Allow for whitespace before the expected character.  */
+  skip_whitespace (*str);
+
   if (**str == c)
     {
       (*str)++;
@@ -5168,6 +5171,9 @@ parse_address_main (char **str, int i, i
       return PARSE_OPERAND_SUCCESS;
     }
 
+  /* PR gas/14887: Allow for whitespace after the opening bracket.  */
+  skip_whitespace (p);
+
   if ((reg = arm_reg_parse (&p, REG_TYPE_RN)) == FAIL)
     {
       inst.error = _(reg_expected_msgs[REG_TYPE_RN]);
@@ -22672,6 +22678,12 @@ tc_gen_reloc (asection *section, fixS *f
 	}
 
     case BFD_RELOC_32:
+      if (streq (S_GET_NAME (fixp->fx_addsy), "__GOTT_BASE__"))
+        {
+	  code = BFD_RELOC_NONE;
+	  break;
+	}
+
       if (fixp->fx_pcrel)
 	{
 	  code = BFD_RELOC_32_PCREL;
@@ -22814,6 +22826,13 @@ tc_gen_reloc (asection *section, fixS *f
       return NULL;
 
     case BFD_RELOC_ARM_OFFSET_IMM:
+      /* RISC OS */
+      if (streq (S_GET_NAME (fixp->fx_addsy), "__GOTT_INDEX__"))
+	{
+	  code = BFD_RELOC_NONE;
+	  break;
+	}
+
       if (section->use_rela_p)
 	{
 	  code = fixp->fx_r_type;
@@ -25096,3 +25115,13 @@ arm_apply_sym_value (struct fix * fixP)
   return 0;
 }
 #endif /* OBJ_ELF */
+
+void riscos_symbol_new_hook (symbolS *sym)
+{
+  const char *sym_name = S_GET_NAME (sym);
+
+  if (sym_name[0] == '_' && sym_name[1] == '_'
+   && (strcmp (sym_name, "__GOTT_BASE__") == 0
+    || strcmp (sym_name, "__GOTT_INDEX__") == 0))
+    S_SET_WEAK (sym);
+}
