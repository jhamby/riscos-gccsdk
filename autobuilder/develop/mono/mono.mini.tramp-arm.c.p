--- mono/mini/tramp-arm.c.orig	2013-02-27 16:56:56.000000000 +0000
+++ mono/mini/tramp-arm.c	2013-03-04 20:10:57.000000000 +0000
@@ -828,7 +828,18 @@
 		mono_marshal_find_bitfield_offset (MonoVTable, initialized, &byte_offset, &bitmask);
 
 	g_assert (arm_is_imm8 (byte_offset));
+#ifdef __riscos__
+	/* The LDRSB instruction will not work in RPCEmu.  */
+	if (v5_supported) {
+		ARM_LDRSB_IMM (code, ARMREG_IP, MONO_ARCH_VTABLE_REG, byte_offset);
+	} else {
+		ARM_LDRB_IMM (code, ARMREG_IP, MONO_ARCH_VTABLE_REG, byte_offset);
+		ARM_SHL_IMM (code, ARMREG_IP, ARMREG_IP, 24);
+		ARM_SAR_IMM (code, ARMREG_IP, ARMREG_IP, 24);
+	}
+#else
 	ARM_LDRSB_IMM (code, ARMREG_IP, MONO_ARCH_VTABLE_REG, byte_offset);
+#endif
 	imm8 = mono_arm_is_rotated_imm8 (bitmask, &rot_amount);
 	g_assert (imm8 >= 0);
 	ARM_AND_REG_IMM (code, ARMREG_IP, ARMREG_IP, imm8, rot_amount);
