--- gcc/config/arm/arm.c.orig	2018-03-27 15:06:20.000000000 +0100
+++ gcc/config/arm/arm.c	2019-02-17 20:31:53.997737792 +0000
@@ -3469,6 +3469,12 @@
        it.  */
     target_flags |= MASK_SINGLE_PIC_BASE;
 
+#ifdef TARGET_RISCOSELF
+  /* The data segment is stored seperately from the text segment at an
+   * address determined at runtime.  */
+  arm_pic_data_is_text_relative = 0;
+#endif
+
   /* If stack checking is disabled, we can use r10 as the PIC register,
      which keeps r9 available.  The EABI specifies r9 as the PIC register.  */
   if (flag_pic && TARGET_SINGLE_PIC_BASE)
@@ -7649,6 +7655,17 @@
   gcc_assert (flag_pic);
 
   pic_reg = cfun->machine->pic_reg;
+#ifdef TARGET_RISCOSELF
+  if (flag_pic == 2)
+    {
+      emit_insn (gen_rtx_SET (pic_reg, GEN_INT(0x8000)));
+      emit_insn (gen_pic_offset_arm (pic_reg, pic_reg, GEN_INT(0x38)));
+
+      pic_tmp = gen_rtx_SYMBOL_REF (Pmode, RISCOS_GOTT_INDEX);
+      emit_insn (gen_pic_offset_arm (pic_reg, pic_reg, pic_tmp));
+    }
+  else
+#endif
   if (TARGET_VXWORKS_RTP)
     {
       pic_rtx = gen_rtx_SYMBOL_REF (Pmode, VXWORKS_GOTT_BASE);
@@ -28078,6 +28095,13 @@
   if (cfun->has_nonlocal_label)
     return true;
 
+#ifdef TARGET_RISCOSELF
+  /* For the RISC OS version of ARMEABI, the frame pointer is required
+     for non-leaf functions.  */
+  if (!crtl->is_leaf)
+    return true;
+#endif
+
   /* The frame pointer is required for non-leaf APCS frames.  */
   if (TARGET_ARM && TARGET_APCS_FRAME && !crtl->is_leaf)
     return true;
