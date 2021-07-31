--- mono/mini/mini-arm.h.orig	2013-07-30 02:50:25.000000000 +0100
+++ mono/mini/mini-arm.h	2013-08-02 20:24:44.831899124 +0100
@@ -175,6 +175,9 @@
 	gpointer seq_point_bp_method_var;
 	gboolean omit_fp, omit_fp_computed;
 	gpointer cinfo;
+#ifdef ENABLE_RISCOS_STACKFRAMES
+	gboolean stack_frame_needed;
+#endif
 } MonoCompileArch;
 
 #define MONO_ARCH_EMULATE_FCONV_TO_I8 1
@@ -242,7 +245,12 @@
 #endif
 
 /* ARM doesn't have too many registers, so we have to use a callee saved one */
+#ifdef ENABLE_RISCOS_STACKFRAMES
+/* V5 is corrupted by PLTs on RISC OS.  */
+#define MONO_ARCH_RGCTX_REG ARMREG_V4
+#else
 #define MONO_ARCH_RGCTX_REG ARMREG_V5
+#endif
 /* First argument reg */
 #define MONO_ARCH_VTABLE_REG ARMREG_R0
 
@@ -288,4 +296,11 @@
 mono_arm_load_jumptable_entry (guint8 *code, gpointer *jte, ARMReg reg) MONO_INTERNAL;
 #endif
 
+#ifdef ENABLE_RISCOS_STACKFRAMES
+extern void (*__rt_stkovf_split_small)(void);
+extern void (*__rt_stkovf_split_big)(void);
+#endif
+
+extern int v5_supported;
+
 #endif /* __MONO_MINI_ARM_H__ */
