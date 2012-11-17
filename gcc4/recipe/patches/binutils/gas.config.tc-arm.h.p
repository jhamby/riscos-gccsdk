--- gas/config/tc-arm.h.orig	2012-09-04 14:53:45.000000000 +0200
+++ gas/config/tc-arm.h	2012-11-17 14:55:35.901614031 +0100
@@ -347,6 +347,9 @@ extern int arm_elf_section_type (const c
 extern int tc_arm_regname_to_dw2regnum (char *regname);
 extern void tc_arm_frame_initial_instructions (void);
 
+extern void riscos_symbol_new_hook (symbolS *);
+#define tc_symbol_new_hook(sym)		riscos_symbol_new_hook (sym)
+
 #ifdef TE_PE
 
 #define O_secrel O_md1
