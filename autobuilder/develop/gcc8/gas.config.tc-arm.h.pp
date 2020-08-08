--- gas/config/tc-arm.h.orig	2018-01-13 13:31:15.000000000 +0000
+++ gas/config/tc-arm.h	2019-01-15 13:33:23.693995258 +0000
@@ -375,6 +375,9 @@
 extern int tc_arm_regname_to_dw2regnum (char *regname);
 extern void tc_arm_frame_initial_instructions (void);
 
+extern void riscos_symbol_new_hook (symbolS *);
+#define tc_symbol_new_hook(sym)		riscos_symbol_new_hook (sym)
+
 #ifdef TE_PE
 
 #define O_secrel O_md1
