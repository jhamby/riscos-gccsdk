--- gas/config/tc-arm.c.orig	2013-11-26 12:37:33.000000000 +0100
+++ gas/config/tc-arm.c	2013-12-31 13:51:51.279539763 +0100
@@ -241,7 +241,10 @@ static const arm_feature_set fpu_crypto_
 static const arm_feature_set crc_ext_armv8 =
   ARM_FEATURE (0, CRC_EXT_ARMV8);
 
-static int mfloat_abi_opt = -1;
+#ifndef DEFAULT_FLOAT_ABI
+#define DEFAULT_FLOAT_ABI (-1)
+#endif
+static int mfloat_abi_opt = DEFAULT_FLOAT_ABI;
 /* Record user cpu selection for object attributes.  */
 static arm_feature_set selected_cpu = ARM_ARCH_NONE;
 /* Must be long enough to hold any of the names in arm_cpus.  */
@@ -22771,6 +22774,13 @@ tc_gen_reloc (asection *section, fixS *f
 	}
 
     case BFD_RELOC_32:
+      /* RISC OS */
+      if (streq (S_GET_NAME (fixp->fx_addsy), "__GOTT_BASE__"))
+	{
+	  code = BFD_RELOC_NONE;
+	  break;
+	}
+
       if (fixp->fx_pcrel)
 	{
 	  code = BFD_RELOC_32_PCREL;
@@ -22913,6 +22923,13 @@ tc_gen_reloc (asection *section, fixS *f
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
@@ -25204,3 +25221,14 @@ arm_apply_sym_value (struct fix * fixP)
   return 0;
 }
 #endif /* OBJ_ELF */
+
+void
+riscos_symbol_new_hook (symbolS *sym)
+{
+  const char *sym_name = S_GET_NAME (sym);
+
+  if (sym_name[0] == '_' && sym_name[1] == '_'
+      && (strcmp (sym_name, "__GOTT_BASE__") == 0
+	  || strcmp (sym_name, "__GOTT_INDEX__") == 0))
+    S_SET_WEAK (sym);
+}
