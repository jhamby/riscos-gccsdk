--- ld/emultempl/armelf.em.orig	2013-03-25 09:06:23.000000000 +0100
+++ ld/emultempl/armelf.em	2013-03-26 23:24:38.745493980 +0100
@@ -43,6 +43,7 @@ static int no_wchar_size_warning = 0;
 static int pic_veneer = 0;
 static int merge_exidx_entries = -1;
 static int fix_arm1176 = 1;
+static int riscos_module = 0;
 
 static void
 gld${EMULATION_NAME}_before_parse (void)
@@ -56,6 +57,28 @@ gld${EMULATION_NAME}_before_parse (void)
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
+  if (link_info.shared && link_info.flag_pic == 2)
+    {
+      if (!bfd_elf32_arm_create_riscos_pic_section (&link_info))
+        einfo (_("%P: failed to create RISC OS PIC section\n"));
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
@@ -88,8 +111,30 @@ arm_elf_before_allocation (void)
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
+
+  if (link_info.shared && link_info.flag_pic == 2)
+    {
+      if (!bfd_elf32_arm_allocate_riscos_pic_section (&link_info))
+	{
+	  einfo (_("%P: failed to allocate RISC OS PIC section\n"));
+	  return;
+	}
+    }
 }
 
 /* Fake input file for stubs.  */
@@ -534,6 +579,9 @@ PARSE_AND_LIST_PROLOGUE='
 #define OPTION_NO_MERGE_EXIDX_ENTRIES   316
 #define OPTION_FIX_ARM1176		317
 #define OPTION_NO_FIX_ARM1176		318
+#define OPTION_FPIC_1			319
+#define OPTION_FPIC_2			320
+#define OPTION_RISCOS_MODULE		321
 '
 
 PARSE_AND_LIST_SHORTOPTS=p
@@ -558,6 +606,9 @@ PARSE_AND_LIST_LONGOPTS='
   { "no-merge-exidx-entries", no_argument, NULL, OPTION_NO_MERGE_EXIDX_ENTRIES },
   { "fix-arm1176", no_argument, NULL, OPTION_FIX_ARM1176 },
   { "no-fix-arm1176", no_argument, NULL, OPTION_NO_FIX_ARM1176 },
+  { "fpic", no_argument, NULL, OPTION_FPIC_1},
+  { "fPIC", no_argument, NULL, OPTION_FPIC_2},
+  { "ro-module-reloc", no_argument, NULL, OPTION_RISCOS_MODULE},
 '
 
 PARSE_AND_LIST_OPTIONS='
@@ -587,6 +638,9 @@ PARSE_AND_LIST_OPTIONS='
   fprintf (file, _("  --[no-]fix-cortex-a8        Disable/enable Cortex-A8 Thumb-2 branch erratum fix\n"));
   fprintf (file, _("  --no-merge-exidx-entries    Disable merging exidx entries\n"));
   fprintf (file, _("  --[no-]fix-arm1176          Disable/enable ARM1176 BLX immediate erratum fix\n"));
+  fprintf (file, _("  -fpic                       Generate original PLT entries\n"));
+  fprintf (file, _("  -fPIC                       Generate RISC OS PLT entries\n"));
+  fprintf (file, _("  --ro-module-reloc           Add RISC OS module relocation code & data\n"));
 '
 
 PARSE_AND_LIST_ARGS_CASES='
@@ -678,8 +732,23 @@ PARSE_AND_LIST_ARGS_CASES='
    case OPTION_NO_FIX_ARM1176:
       fix_arm1176 = 0;
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
 
+# To add __RelocCode section for RISC OS modules.
+LDEMUL_AFTER_OPEN=arm_elf_after_open
+
 # We have our own before_allocation etc. functions, but they call
 # the standard routines, so give them a different name.
 LDEMUL_BEFORE_ALLOCATION=arm_elf_before_allocation
