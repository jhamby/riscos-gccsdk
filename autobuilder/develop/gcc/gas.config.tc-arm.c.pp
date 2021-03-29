--- gas/config/tc-arm.c.orig	2018-01-17 08:12:50.000000000 +0000
+++ gas/config/tc-arm.c	2020-05-14 21:25:57.992171851 +0100
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
@@ -24779,7 +24793,14 @@
       as_bad_where (fixp->fx_file, fixp->fx_line,
 		    _("internal_relocation (type: OFFSET_IMM) not fixed up"));
       return NULL;
-
+    case BFD_RELOC_ARM_T32_OFFSET_IMM:
+      /* RISC OS */
+      if (streq (S_GET_NAME (fixp->fx_addsy), "__GOTT_THUMB_INDEX__"))
+	{
+	  code = BFD_RELOC_NONE;
+	  break;
+	}
+	/* Fall through.  */
     default:
       {
 	const char * type;
@@ -27666,4 +27687,17 @@
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
+          || strcmp (sym_name, "__GOTT_THUMB_INDEX__") == 0
+	  || strcmp (sym_name, "__GOTT_INDEX__") == 0))
+    S_SET_WEAK (sym);
+}
+
 #endif /* OBJ_ELF */
