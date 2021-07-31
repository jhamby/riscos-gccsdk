--- ld/emultempl/armelf.em.orig	2018-01-13 13:31:16.000000000 +0000
+++ ld/emultempl/armelf.em	2019-01-14 22:04:47.957398766 +0000
@@ -45,7 +45,8 @@
   1,				/* fix_arm1176 */
   -1,				/* merge_exidx_entries */
   0,				/* cmse_implib */
-  NULL				/* in_implib_bfd */
+  NULL,				/* in_implib_bfd */
+  0				/* riscos_module */
 };
 static char *in_implib_filename = NULL;
 
@@ -76,6 +77,28 @@
 }
 
 static void
+arm_elf_after_open (void)
+{
+  /* Add __RelocCode code section to the first input bfd struct we find. This section
+     can be picked up by our linker script. */
+  if (params.riscos_module
+      && !bfd_elf32_arm_create_riscos_module_reloc (((lang_input_statement_type *)file_chain.head)->the_bfd, &link_info))
+    {
+      einfo (_("%P: failed to create RISC OS module relocation code\n"));
+      params.riscos_module = 0;
+    }
+
+  if (bfd_link_pic(&link_info) && link_info.flag_pic == 2)
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
   bfd_elf32_arm_set_byteswap_code (&link_info, params.byteswap_code);
@@ -118,8 +141,29 @@
       bfd_elf32_arm_allocate_interworking_sections (& link_info);
     }
 
+  /* Allocate the right size of our __RelocCode section based on the
+     relocations in all our output bfd's.  */
+  if (params.riscos_module)
+    {
+      if (!bfd_elf32_arm_allocate_riscos_module_reloc (&link_info))
+	{
+	  einfo (_("%P: failed to allocate RISC OS module relocation code\n"));
+	  params.riscos_module = 0;
+	  return;
+	}
+    }
+
   /* Call the standard elf routine.  */
   gld${EMULATION_NAME}_before_allocation ();
+
+  if (bfd_link_pic(&link_info) && link_info.flag_pic == 2)
+    {
+      if (!bfd_elf32_arm_allocate_riscos_pic_section (&link_info))
+	{
+	  einfo (_("%P: failed to allocate RISC OS PIC section\n"));
+	  return;
+	}
+    }
 }
 
 /* Fake input file for stubs.  */
@@ -600,6 +644,10 @@
 #define OPTION_STM32L4XX_FIX		320
 #define OPTION_CMSE_IMPLIB		321
 #define OPTION_IN_IMPLIB		322
+#define OPTION_FPIC_1			340
+#define OPTION_FPIC_2			341
+#define OPTION_RISCOS_MODULE		342
+#define OPTION_RISCOS_ABI		343
 '
 
 PARSE_AND_LIST_SHORTOPTS=p
@@ -628,6 +676,10 @@
   { "long-plt", no_argument, NULL, OPTION_LONG_PLT },
   { "cmse-implib", no_argument, NULL, OPTION_CMSE_IMPLIB },
   { "in-implib", required_argument, NULL, OPTION_IN_IMPLIB },
+  { "fpic", no_argument, NULL, OPTION_FPIC_1 },
+  { "fPIC", no_argument, NULL, OPTION_FPIC_2 },
+  { "ro-module-reloc", no_argument, NULL, OPTION_RISCOS_MODULE },
+  { "riscos-abi", required_argument, NULL, OPTION_RISCOS_ABI },
 '
 
 PARSE_AND_LIST_OPTIONS='
@@ -664,6 +716,10 @@
   fprintf (file, _("  --[no-]fix-cortex-a8        Disable/enable Cortex-A8 Thumb-2 branch erratum fix\n"));
   fprintf (file, _("  --no-merge-exidx-entries    Disable merging exidx entries\n"));
   fprintf (file, _("  --[no-]fix-arm1176          Disable/enable ARM1176 BLX immediate erratum fix\n"));
+  fprintf (file, _("  -fpic                       Generate original PLT entries\n"));
+  fprintf (file, _("  -fPIC                       Generate RISC OS PLT entries\n"));
+  fprintf (file, _("  --ro-module-reloc           Add RISC OS module relocation code & data\n"));
+  fprintf (file, _("  --riscos-abi <ABI>          Use ABI as the contents of the .riscos.abi.version section.\n"));
 '
 
 PARSE_AND_LIST_ARGS_CASES='
@@ -780,8 +836,27 @@
    case OPTION_IN_IMPLIB:
       in_implib_filename = optarg;
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
+      params.riscos_module = 1;
+      break;
+
+   case OPTION_RISCOS_ABI:
+      link_info.riscos_abi = optarg;
+      break;
 '
 
+# To add __RelocCode section for RISC OS modules.
+LDEMUL_AFTER_OPEN=arm_elf_after_open
+
 # We have our own before_allocation etc. functions, but they call
 # the standard routines, so give them a different name.
 LDEMUL_BEFORE_ALLOCATION=arm_elf_before_allocation
