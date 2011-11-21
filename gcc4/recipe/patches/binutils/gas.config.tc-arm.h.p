--- gas/config/tc-arm.h.orig	2011-05-31 15:40:04.000000000 +0200
+++ gas/config/tc-arm.h	2011-11-21 19:42:18.658237164 +0100
@@ -339,6 +339,9 @@ extern int arm_elf_section_type (const c
 extern int tc_arm_regname_to_dw2regnum (char *regname);
 extern void tc_arm_frame_initial_instructions (void);
 
+extern void riscos_symbol_new_hook (symbolS *);
+#define tc_symbol_new_hook(sym)		riscos_symbol_new_hook (sym)
+
 #ifdef TE_PE
 
 #define O_secrel O_md1
