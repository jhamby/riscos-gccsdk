--- ld/emultempl/armelf.em.orig	2010-01-31 22:55:49.605013495 +0100
+++ ld/emultempl/armelf.em	2010-02-02 02:44:57.245033622 +0100
@@ -41,6 +41,7 @@ static int fix_cortex_a8 = -1;
 static int no_enum_size_warning = 0;
 static int no_wchar_size_warning = 0;
 static int pic_veneer = 0;
+static int riscos_module = 0;
 
 static void
 gld${EMULATION_NAME}_before_parse (void)
@@ -53,6 +54,22 @@ gld${EMULATION_NAME}_before_parse (void)
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
@@ -85,6 +102,19 @@ arm_elf_before_allocation (void)
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
@@ -525,6 +555,9 @@ PARSE_AND_LIST_PROLOGUE='
 #define OPTION_NO_WCHAR_SIZE_WARNING	313
 #define OPTION_FIX_CORTEX_A8		314
 #define OPTION_NO_FIX_CORTEX_A8		315
+#define OPTION_FPIC_1			316
+#define OPTION_FPIC_2			317
+#define OPTION_RISCOS_MODULE		318
 '
 
 PARSE_AND_LIST_SHORTOPTS=p
@@ -546,6 +579,9 @@ PARSE_AND_LIST_LONGOPTS='
   { "no-wchar-size-warning", no_argument, NULL, OPTION_NO_WCHAR_SIZE_WARNING},
   { "fix-cortex-a8", no_argument, NULL, OPTION_FIX_CORTEX_A8 },
   { "no-fix-cortex-a8", no_argument, NULL, OPTION_NO_FIX_CORTEX_A8 },
+  { "fpic", no_argument, NULL, OPTION_FPIC_1},
+  { "fPIC", no_argument, NULL, OPTION_FPIC_2},
+  { "ro-module-reloc", no_argument, NULL, OPTION_RISCOS_MODULE},
 '
 
 PARSE_AND_LIST_OPTIONS='
@@ -573,6 +609,9 @@ PARSE_AND_LIST_OPTIONS='
                            the linker should choose suitable defaults.\n"
  		   ));
   fprintf (file, _("  --[no-]fix-cortex-a8        Disable/enable Cortex-A8 Thumb-2 branch erratum fix\n"));
+  fprintf (file, _("      -fpic                   Generate original PLT entries\n"));
+  fprintf (file, _("      -fPIC                   Generate RISC OS PLT entries\n"));
+  fprintf (file, _("     --ro-module-reloc        Add RISC OS module relocation code & data\n"));
 '
 
 PARSE_AND_LIST_ARGS_CASES='
@@ -652,10 +691,23 @@ PARSE_AND_LIST_ARGS_CASES='
     case OPTION_NO_FIX_CORTEX_A8:
       fix_cortex_a8 = 0;
       break;
+
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
 '
 
 # We have our own before_allocation etc. functions, but they call
 # the standard routines, so give them a different name.
+LDEMUL_AFTER_OPEN=arm_elf_after_open
 LDEMUL_BEFORE_ALLOCATION=arm_elf_before_allocation
 LDEMUL_AFTER_ALLOCATION=gld${EMULATION_NAME}_after_allocation
 LDEMUL_CREATE_OUTPUT_SECTION_STATEMENTS=arm_elf_create_output_section_statements
