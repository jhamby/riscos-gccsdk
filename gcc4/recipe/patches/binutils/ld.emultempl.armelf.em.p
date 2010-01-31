--- ld/emultempl/armelf.em.orig	2007-03-04 03:51:48.000000000 +0100
+++ ld/emultempl/armelf.em	2007-02-19 02:10:06.000000000 +0100
@@ -35,6 +35,7 @@
 static char *target2_type = "${TARGET2_TYPE}";
 static int fix_v4bx = 0;
 static int use_blx = 0;
+static int riscos_module = 0;
 
 static void
 gld${EMULATION_NAME}_before_parse (void)
@@ -66,6 +67,15 @@
       }
   }
 
+  /* Add __RelocCode code section to the first input bfd struct we find. This section
+     can be picked up by our linker script. */
+  if (riscos_module
+      && !bfd_elf32_arm_create_riscos_module_reloc (((lang_input_statement_type *)file_chain.head)->the_bfd, &link_info))
+    {
+      einfo (_("%P: failed to create RISC OS module relocation code\n"));
+      riscos_module = 0;
+    }
+
   /* Call the standard elf routine.  */
   gld${EMULATION_NAME}_after_open ();
 }
@@ -142,6 +152,18 @@
 
   /* We have seen it all. Allocate it, and carry on.  */
   bfd_elf32_arm_allocate_interworking_sections (& link_info);
+
+  /* Allocate the right size of our __RelocCode section based on the
+     relocations in all our output bfd's.  */
+  if (riscos_module)
+    {
+      if (!bfd_elf32_arm_allocate_riscos_module_reloc (&link_info))
+	{
+	  einfo (_("%P: failed to allocate RISC OS module relocation code\n"));
+	  riscos_module = 0;
+          return;
+	}
+    }
 }
 
 static void
@@ -229,6 +251,9 @@
 #define OPTION_TARGET2			305
 #define OPTION_FIX_V4BX			306
 #define OPTION_USE_BLX			307
+#define OPTION_FPIC_1			308
+#define OPTION_FPIC_2			309
+#define OPTION_RISCOS_MODULE		310
 '
 
 PARSE_AND_LIST_SHORTOPTS=p
@@ -242,6 +267,9 @@
   { "target2", required_argument, NULL, OPTION_TARGET2},
   { "fix-v4bx", no_argument, NULL, OPTION_FIX_V4BX},
   { "use-blx", no_argument, NULL, OPTION_USE_BLX},
+  { "fpic", no_argument, NULL, OPTION_FPIC_1},
+  { "fPIC", no_argument, NULL, OPTION_FPIC_2},
+  { "ro-module-reloc", no_argument, NULL, OPTION_RISCOS_MODULE},
 '
 
 PARSE_AND_LIST_OPTIONS='
@@ -252,6 +280,9 @@
   fprintf (file, _("     --target2=<type>         Specify definition of R_ARM_TARGET2\n"));
   fprintf (file, _("     --fix-v4bx               Rewrite BX rn as MOV pc, rn for ARMv4\n"));
   fprintf (file, _("     --use-blx                Enable use of BLX instructions\n"));
+  fprintf (file, _("      -fpic                   Generate original PLT entries\n"));
+  fprintf (file, _("      -fPIC                   Generate RISC OS PLT entries\n"));
+  fprintf (file, _("     --ro-module-reloc        Add RISC OS module relocation code & data\n"));
 '
 
 PARSE_AND_LIST_ARGS_CASES='
@@ -286,6 +317,18 @@
     case OPTION_USE_BLX:
       use_blx = 1;
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
 
 # We have our own after_open and before_allocation functions, but they call
