--- gas/config/tc-arm.c.orig	2013-06-15 14:28:31.000000000 +0100
+++ gas/config/tc-arm.c	2013-06-15 14:17:06.000000000 +0100
@@ -241,7 +241,10 @@ static const arm_feature_set fpu_neon_ex
 static const arm_feature_set fpu_crypto_ext_armv8 =
   ARM_FEATURE (0, FPU_CRYPTO_EXT_ARMV8);
 
-static int mfloat_abi_opt = -1;
+#ifndef DEFAULT_FLOAT_ABI
+#define DEFAULT_FLOAT_ABI (-1)
+#endif
+static int mfloat_abi_opt = DEFAULT_FLOAT_ABI;
 /* Record user cpu selection for object attributes.  */
 static arm_feature_set selected_cpu = ARM_ARCH_NONE;
 /* Must be long enough to hold any of the names in arm_cpus.  */
@@ -885,6 +888,9 @@ const char FLT_CHARS[] = "rRsSfFdDxXeEpP
 static inline int
 skip_past_char (char ** str, char c)
 {
+  /* PR gas/14987: Allow for whitespace before the expected character.  */
+  skip_whitespace (*str);
+
   if (**str == c)
     {
       (*str)++;
@@ -5168,6 +5174,9 @@ parse_address_main (char **str, int i, i
       return PARSE_OPERAND_SUCCESS;
     }
 
+  /* PR gas/14887: Allow for whitespace after the opening bracket.  */
+  skip_whitespace (p);
+
   if ((reg = arm_reg_parse (&p, REG_TYPE_RN)) == FAIL)
     {
       inst.error = _(reg_expected_msgs[REG_TYPE_RN]);
@@ -22672,6 +22681,12 @@ tc_gen_reloc (asection *section, fixS *f
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
@@ -22814,6 +22829,13 @@ tc_gen_reloc (asection *section, fixS *f
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
@@ -25096,3 +25118,13 @@ arm_apply_sym_value (struct fix * fixP)
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
