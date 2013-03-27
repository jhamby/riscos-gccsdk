--- gas/config/tc-arm.c.orig	2013-03-25 09:06:21.000000000 +0100
+++ gas/config/tc-arm.c	2013-03-26 23:24:38.433497903 +0100
@@ -22672,6 +22672,12 @@ tc_gen_reloc (asection *section, fixS *f
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
@@ -22814,6 +22820,13 @@ tc_gen_reloc (asection *section, fixS *f
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
@@ -25096,3 +25109,13 @@ arm_apply_sym_value (struct fix * fixP)
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
