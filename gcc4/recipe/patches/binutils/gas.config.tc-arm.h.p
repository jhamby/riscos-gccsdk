--- gas/config/tc-arm.h.orig	2011-07-23 13:54:40.000000000 +0100
+++ gas/config/tc-arm.h	2011-07-20 20:28:43.000000000 +0100
@@ -333,6 +333,9 @@ extern int arm_elf_section_type (const c
 extern int tc_arm_regname_to_dw2regnum (char *regname);
 extern void tc_arm_frame_initial_instructions (void);
 
+extern void riscos_symbol_new_hook (symbolS *);
+#define tc_symbol_new_hook(sym)		riscos_symbol_new_hook (sym)
+
 #ifdef TE_PE
 
 #define O_secrel O_md1
