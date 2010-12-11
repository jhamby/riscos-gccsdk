--- ld/emultempl/armelf.em.orig	2010-04-21 18:32:31.000000000 +0200
+++ ld/emultempl/armelf.em	2010-12-11 02:16:55.734993048 +0100
@@ -42,6 +42,7 @@ static int no_enum_size_warning = 0;
 static int no_wchar_size_warning = 0;
 static int pic_veneer = 0;
 static int merge_exidx_entries = -1;
+static int riscos_module = 0;
 
 static void
 gld${EMULATION_NAME}_before_parse (void)
@@ -54,6 +55,22 @@ gld${EMULATION_NAME}_before_parse (void)
 }
 
 static void
+arm_elf_after_open (void)
+{
+  /* Add __RelocCode code section to the first input bfd struct we find. This section
+     can be picked up by our linker script. */
+  if (riscos_module
+      && !bfd_elf32_arm_create_riscos_module_reloc (((lang_input_statement_type *)file_chain.head)->the_bfd, &link_info))
+    {
+      einfo (_("%P: failed to create RISC OS module relocation code\n"));
+      riscos_module = 0;
+    }
+
+  /* Call the standard elf routine.  */
+  gld${EMULATION_NAME}_after_open ();
+}
+
+static void
 arm_elf_before_allocation (void)
 {
   bfd_elf32_arm_set_byteswap_code (&link_info, byteswap_code);
@@ -86,6 +103,19 @@ arm_elf_before_allocation (void)
       bfd_elf32_arm_allocate_interworking_sections (& link_info);
     }
 
+
+  /* Allocate the right size of our __RelocCode section based on the
+     relocations in all our output bfd's.  */
+  if (riscos_module)
+    {
+      if (!bfd_elf32_arm_allocate_riscos_module_reloc (&link_info))
+	{
+	  einfo (_("%P: failed to allocate RISC OS module relocation code\n"));
+	  riscos_module = 0;
+	  return;
+	}
+    }
+
   /* Call the standard elf routine.  */
   gld${EMULATION_NAME}_before_allocation ();
 }
@@ -529,6 +559,9 @@ PARSE_AND_LIST_PROLOGUE='
 #define OPTION_FIX_CORTEX_A8		314
 #define OPTION_NO_FIX_CORTEX_A8		315
 #define OPTION_NO_MERGE_EXIDX_ENTRIES   316
+#define OPTION_FPIC_1			317
+#define OPTION_FPIC_2			318
+#define OPTION_RISCOS_MODULE		319
 '
 
 PARSE_AND_LIST_SHORTOPTS=p
@@ -551,6 +584,9 @@ PARSE_AND_LIST_LONGOPTS='
   { "fix-cortex-a8", no_argument, NULL, OPTION_FIX_CORTEX_A8 },
   { "no-fix-cortex-a8", no_argument, NULL, OPTION_NO_FIX_CORTEX_A8 },
   { "no-merge-exidx-entries", no_argument, NULL, OPTION_NO_MERGE_EXIDX_ENTRIES },
+  { "fpic", no_argument, NULL, OPTION_FPIC_1},
+  { "fPIC", no_argument, NULL, OPTION_FPIC_2},
+  { "ro-module-reloc", no_argument, NULL, OPTION_RISCOS_MODULE},
 '
 
 PARSE_AND_LIST_OPTIONS='
@@ -579,6 +615,9 @@ PARSE_AND_LIST_OPTIONS='
  		   ));
   fprintf (file, _("  --[no-]fix-cortex-a8        Disable/enable Cortex-A8 Thumb-2 branch erratum fix\n"));
   fprintf (file, _("  --no-merge-exidx-entries    Disable merging exidx entries\n"));
+  fprintf (file, _("  -fpic                       Generate original PLT entries\n"));
+  fprintf (file, _("  -fPIC                       Generate RISC OS PLT entries\n"));
+  fprintf (file, _("  --ro-module-reloc           Add RISC OS module relocation code & data\n"));
  
 '
 
@@ -663,8 +702,23 @@ PARSE_AND_LIST_ARGS_CASES='
    case OPTION_NO_MERGE_EXIDX_ENTRIES:
       merge_exidx_entries = 0;
 
+    case OPTION_FPIC_1:
+      link_info.flag_pic = 1;
+      break;
+
+    case OPTION_FPIC_2:
+      link_info.flag_pic = 2;
+      break;
+
+    case OPTION_RISCOS_MODULE:
+      riscos_module = 1;
+      break;
+
 '
 
+# To add __RelocCode section for RISC OS modules.
+LDEMUL_AFTER_OPEN=arm_elf_after_open
+
 # We have our own before_allocation etc. functions, but they call
 # the standard routines, so give them a different name.
 LDEMUL_BEFORE_ALLOCATION=arm_elf_before_allocation
