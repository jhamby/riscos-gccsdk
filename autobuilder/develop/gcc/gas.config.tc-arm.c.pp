--- gas/config/tc-arm.c.orig	2018-01-17 08:12:50.000000000 +0000
+++ gas/config/tc-arm.c	2019-01-15 13:30:06.005992697 +0000
@@ -24609,6 +24609,13 @@
       /* Fall through.  */
 
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
@@ -24760,6 +24767,13 @@
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
@@ -27666,4 +27680,16 @@
     }
   return 0;
 }
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
+
 #endif /* OBJ_ELF */
