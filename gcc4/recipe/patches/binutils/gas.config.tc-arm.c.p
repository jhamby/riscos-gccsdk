--- gas/config/tc-arm.c.orig	2011-11-21 10:29:31.000000000 +0100
+++ gas/config/tc-arm.c	2011-11-21 19:42:18.654237215 +0100
@@ -21723,6 +21723,12 @@ tc_gen_reloc (asection *section, fixS *f
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
@@ -21865,6 +21871,13 @@ tc_gen_reloc (asection *section, fixS *f
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
@@ -24061,3 +24074,13 @@ arm_apply_sym_value (struct fix * fixP)
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
