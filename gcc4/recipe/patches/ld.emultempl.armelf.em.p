--- ld/emultempl/armelf.em.orig	2006-10-19 16:59:54.000000000 +0100
+++ ld/emultempl/armelf.em	2006-10-19 20:30:13.000000000 +0100
@@ -229,6 +229,8 @@
 #define OPTION_TARGET2			305
 #define OPTION_FIX_V4BX			306
 #define OPTION_USE_BLX			307
+#define OPTION_FPIC_1			308
+#define OPTION_FPIC_2			309
 '
 
 PARSE_AND_LIST_SHORTOPTS=p
@@ -242,6 +244,8 @@
   { "target2", required_argument, NULL, OPTION_TARGET2},
   { "fix-v4bx", no_argument, NULL, OPTION_FIX_V4BX},
   { "use-blx", no_argument, NULL, OPTION_USE_BLX},
+  { "fpic", no_argument, NULL, OPTION_FPIC_1},
+  { "fPIC", no_argument, NULL, OPTION_FPIC_2},
 '
 
 PARSE_AND_LIST_OPTIONS='
@@ -252,6 +256,8 @@
   fprintf (file, _("     --target2=<type>         Specify definition of R_ARM_TARGET2\n"));
   fprintf (file, _("     --fix-v4bx               Rewrite BX rn as MOV pc, rn for ARMv4\n"));
   fprintf (file, _("     --use-blx                Enable use of BLX instructions\n"));
+  fprintf (file, _("      -fpic                   Generate original PLT entries\n"));
+  fprintf (file, _("      -fPIC                   Generate RISC OS PLT entries\n"));
 '
 
 PARSE_AND_LIST_ARGS_CASES='
@@ -286,6 +292,14 @@
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
 '
 
 # We have our own after_open and before_allocation functions, but they call
