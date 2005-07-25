Index: libavcodec/armv4l/dsputil_arm_s.S
===================================================================
RCS file: /cvsroot/ffmpeg/ffmpeg/libavcodec/armv4l/dsputil_arm_s.S,v
retrieving revision 1.1
diff -u -r1.1 dsputil_arm_s.S
--- libavcodec/armv4l/dsputil_arm_s.S	26 May 2005 14:32:46 -0000	1.1
+++ libavcodec/armv4l/dsputil_arm_s.S	21 Jul 2005 04:57:28 -0000
@@ -1,97 +1,106 @@
-@
-@ ARMv4L optimized DSP utils
-@ Copyright (c) 2004 AGAWA Koji <i (AT) atty (DOT) jp>
-@
-@ This library is free software; you can redistribute it and/or
-@ modify it under the terms of the GNU Lesser General Public
-@ License as published by the Free Software Foundation; either
-@ version 2 of the License, or (at your option) any later version.
-@
-@ This library is distributed in the hope that it will be useful,
-@ but WITHOUT ANY WARRANTY; without even the implied warranty of
-@ MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
-@ Lesser General Public License for more details.
-@
-@ You should have received a copy of the GNU Lesser General Public
-@ License along with this library; if not, write to the Free Software
-@ Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
-@
-
-.macro  ADJ_ALIGN_QUADWORD_D shift, Rd0, Rd1, Rd2, Rd3, Rn0, Rn1, Rn2, Rn3, Rn4
-        mov \Rd0, \Rn0, lsr #(\shift * 8)
-        mov \Rd1, \Rn1, lsr #(\shift * 8)
-        mov \Rd2, \Rn2, lsr #(\shift * 8)
-        mov \Rd3, \Rn3, lsr #(\shift * 8)
-        orr \Rd0, \Rd0, \Rn1, lsl #(32 - \shift * 8)
-        orr \Rd1, \Rd1, \Rn2, lsl #(32 - \shift * 8)
-        orr \Rd2, \Rd2, \Rn3, lsl #(32 - \shift * 8)
-        orr \Rd3, \Rd3, \Rn4, lsl #(32 - \shift * 8)
-.endm
-.macro  ADJ_ALIGN_DOUBLEWORD shift, R0, R1, R2
-        mov \R0, \R0, lsr #(\shift * 8)
-        orr \R0, \R0, \R1, lsl #(32 - \shift * 8)
-        mov \R1, \R1, lsr #(\shift * 8)
-        orr \R1, \R1, \R2, lsl #(32 - \shift * 8)
-.endm
-.macro  ADJ_ALIGN_DOUBLEWORD_D shift, Rdst0, Rdst1, Rsrc0, Rsrc1, Rsrc2
-        mov \Rdst0, \Rsrc0, lsr #(\shift * 8)
-        mov \Rdst1, \Rsrc1, lsr #(\shift * 8)
-        orr \Rdst0, \Rdst0, \Rsrc1, lsl #(32 - (\shift * 8))
-        orr \Rdst1, \Rdst1, \Rsrc2, lsl #(32 - (\shift * 8))
-.endm
-
-.macro  RND_AVG32 Rd0, Rd1, Rn0, Rn1, Rm0, Rm1, Rmask
-        @ Rd = (Rn | Rm) - (((Rn ^ Rm) & ~0x01010101) >> 1)
-        @ Rmask = 0xFEFEFEFE
-        @ Rn = destroy
-        eor \Rd0, \Rn0, \Rm0
-        eor \Rd1, \Rn1, \Rm1
-        orr \Rn0, \Rn0, \Rm0
-        orr \Rn1, \Rn1, \Rm1
-        and \Rd0, \Rd0, \Rmask
-        and \Rd1, \Rd1, \Rmask
-        sub \Rd0, \Rn0, \Rd0, lsr #1
-        sub \Rd1, \Rn1, \Rd1, lsr #1
-.endm
-
-.macro  NO_RND_AVG32 Rd0, Rd1, Rn0, Rn1, Rm0, Rm1, Rmask
-        @ Rd = (Rn & Rm) - (((Rn ^ Rm) & ~0x01010101) >> 1)
-        @ Rmask = 0xFEFEFEFE
-        @ Rn = destroy
-        eor \Rd0, \Rn0, \Rm0
-        eor \Rd1, \Rn1, \Rm1
-        and \Rn0, \Rn0, \Rm0
-        and \Rn1, \Rn1, \Rm1
-        and \Rd0, \Rd0, \Rmask
-        and \Rd1, \Rd1, \Rmask
-        add \Rd0, \Rn0, \Rd0, lsr #1
-        add \Rd1, \Rn1, \Rd1, lsr #1
-.endm
-
-@ ----------------------------------------------------------------
-        .align 8
-        .global put_pixels16_arm
-put_pixels16_arm:
-        @ void func(uint8_t *block, const uint8_t *pixels, int line_size, int h)
-        @ block = word aligned, pixles = unaligned
+;
+; ARMv4L optimized DSP utils
+; Copyright (c) 2004 AGAWA Koji <i (AT) atty (DOT) jp>
+;
+; This library is free software; you can redistribute it and/or
+; modify it under the terms of the GNU Lesser General Public
+; License as published by the Free Software Foundation; either
+; version 2 of the License, or (at your option) any later version.
+;
+; This library is distributed in the hope that it will be useful,
+; but WITHOUT ANY WARRANTY; without even the implied warranty of
+; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
+; Lesser General Public License for more details.
+;
+; You should have received a copy of the GNU Lesser General Public
+; License along with this library; if not, write to the Free Software
+; Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
+;
+
+	AREA |C$$Code|, CODE, READONLY
+
+	MACRO
+	ADJ_ALIGN_QUADWORD_D $shift, $Rd0, $Rd1, $Rd2, $Rd3, $Rn0, $Rn1, $Rn2, $Rn3, $Rn4
+        mov $Rd0, $Rn0, lsr #($shift * 8)
+        mov $Rd1, $Rn1, lsr #($shift * 8)
+        mov $Rd2, $Rn2, lsr #($shift * 8)
+        mov $Rd3, $Rn3, lsr #($shift * 8)
+        orr $Rd0, $Rd0, $Rn1, lsl #(32 - $shift * 8)
+        orr $Rd1, $Rd1, $Rn2, lsl #(32 - $shift * 8)
+        orr $Rd2, $Rd2, $Rn3, lsl #(32 - $shift * 8)
+        orr $Rd3, $Rd3, $Rn4, lsl #(32 - $shift * 8)
+	MEND
+
+	MACRO
+	ADJ_ALIGN_DOUBLEWORD $shift, $R0, $R1, $R2
+        mov $R0, $R0, lsr #($shift * 8)
+        orr $R0, $R0, $R1, lsl #(32 - $shift * 8)
+        mov $R1, $R1, lsr #($shift * 8)
+        orr $R1, $R1, $R2, lsl #(32 - $shift * 8)
+	MEND
+
+	MACRO
+	ADJ_ALIGN_DOUBLEWORD_D $shift, $Rdst0, $Rdst1, $Rsrc0, $Rsrc1, $Rsrc2
+        mov $Rdst0, $Rsrc0, lsr #($shift * 8)
+        mov $Rdst1, $Rsrc1, lsr #($shift * 8)
+        orr $Rdst0, $Rdst0, $Rsrc1, lsl #(32 - ($shift * 8))
+        orr $Rdst1, $Rdst1, $Rsrc2, lsl #(32 - ($shift * 8))
+	MEND
+
+	MACRO
+	RND_AVG32 $Rd0, $Rd1, $Rn0, $Rn1, $Rm0, $Rm1, $Rmask
+        ; Rd = (Rn | Rm) - (((Rn ^ Rm) & ~0x01010101) >> 1)
+        ; Rmask = 0xFEFEFEFE
+        ; Rn = destroy
+        eor $Rd0, $Rn0, $Rm0
+        eor $Rd1, $Rn1, $Rm1
+        orr $Rn0, $Rn0, $Rm0
+        orr $Rn1, $Rn1, $Rm1
+        and $Rd0, $Rd0, $Rmask
+        and $Rd1, $Rd1, $Rmask
+        sub $Rd0, $Rn0, $Rd0, lsr #1
+        sub $Rd1, $Rn1, $Rd1, lsr #1
+	MEND
+
+	MACRO
+	NO_RND_AVG32 $Rd0, $Rd1, $Rn0, $Rn1, $Rm0, $Rm1, $Rmask
+        ; Rd = (Rn & Rm) - (((Rn ^ Rm) & ~0x01010101) >> 1)
+        ; Rmask = 0xFEFEFEFE
+        ; Rn = destroy
+        eor $Rd0, $Rn0, $Rm0
+        eor $Rd1, $Rn1, $Rm1
+        and $Rn0, $Rn0, $Rm0
+        and $Rn1, $Rn1, $Rm1
+        and $Rd0, $Rd0, $Rmask
+        and $Rd1, $Rd1, $Rmask
+        add $Rd0, $Rn0, $Rd0, lsr #1
+        add $Rd1, $Rn1, $Rd1, lsr #1
+	MEND
+
+; ----------------------------------------------------------------
+        ALIGN 8
+        EXPORT put_pixels16_arm
+put_pixels16_arm ROUT
+        ; void func(uint8_t *block, const uint8_t *pixels, int line_size, int h)
+        ; block = word aligned, pixles = unaligned
         pld [r1]
-        stmfd sp!, {r4-r11, lr} @ R14 is also called LR
-        adr r5, 5f
+        stmfd sp!, {r4-r11, lr} ; R14 is also called LR
+        adr r5, %FA05
         ands r4, r1, #3
         bic r1, r1, #3
         add r5, r5, r4, lsl #2
         ldrne pc, [r5]
-1:
+01
         ldmia r1, {r4-r7}
         add r1, r1, r2
         stmia r0, {r4-r7}
         pld [r1]
         subs r3, r3, #1
         add r0, r0, r2
-        bne 1b
+        bne %BA01
         ldmfd sp!, {r4-r11, pc}
-        .align 8
-2:
+        ALIGN 8
+02
         ldmia r1, {r4-r8}
         add r1, r1, r2
         ADJ_ALIGN_QUADWORD_D 1, r9, r10, r11, r12, r4, r5, r6, r7, r8
@@ -99,10 +108,10 @@
         subs r3, r3, #1
         stmia r0, {r9-r12}
         add r0, r0, r2
-        bne 2b
+        bne %BA02
         ldmfd sp!, {r4-r11, pc}
-        .align 8
-3:
+        ALIGN 8
+03
         ldmia r1, {r4-r8}
         add r1, r1, r2
         ADJ_ALIGN_QUADWORD_D 2, r9, r10, r11, r12, r4, r5, r6, r7, r8
@@ -110,10 +119,10 @@
         subs r3, r3, #1
         stmia r0, {r9-r12}
         add r0, r0, r2
-        bne 3b
+        bne %BA03
         ldmfd sp!, {r4-r11, pc}
-        .align 8
-4:
+        ALIGN 8
+04
         ldmia r1, {r4-r8}
         add r1, r1, r2
         ADJ_ALIGN_QUADWORD_D 3, r9, r10, r11, r12, r4, r5, r6, r7, r8
@@ -121,39 +130,39 @@
         subs r3, r3, #1
         stmia r0, {r9-r12}
         add r0, r0, r2
-        bne 4b
+        bne %BA04
         ldmfd sp!, {r4-r11,pc}
-        .align 8
-5:
-        .word 1b
-        .word 2b
-        .word 3b
-        .word 4b
-
-@ ----------------------------------------------------------------
-        .align 8
-        .global put_pixels8_arm
-put_pixels8_arm:
-        @ void func(uint8_t *block, const uint8_t *pixels, int line_size, int h)
-        @ block = word aligned, pixles = unaligned
+        ALIGN 8
+05
+        DCD %BA01
+        DCD %BA02
+        DCD %BA03
+        DCD %BA04
+
+; ----------------------------------------------------------------
+        ALIGN 8
+        EXPORT put_pixels8_arm
+put_pixels8_arm ROUT
+        ; void func(uint8_t *block, const uint8_t *pixels, int line_size, int h)
+        ; block = word aligned, pixles = unaligned
         pld [r1]
-        stmfd sp!, {r4-r5,lr} @ R14 is also called LR
-        adr r5, 5f
+        stmfd sp!, {r4-r5,lr} ; R14 is also called LR
+        adr r5, %FA05
         ands r4, r1, #3
         bic r1, r1, #3
         add r5, r5, r4, lsl #2
         ldrne pc, [r5]
-1:
+01
         ldmia r1, {r4-r5}
         add r1, r1, r2
         subs r3, r3, #1
         pld [r1]
         stmia r0, {r4-r5}
         add r0, r0, r2
-        bne 1b
+        bne %BA01
         ldmfd sp!, {r4-r5,pc}
-        .align 8
-2:
+        ALIGN 8
+02
         ldmia r1, {r4-r5, r12}
         add r1, r1, r2
         ADJ_ALIGN_DOUBLEWORD 1, r4, r5, r12
@@ -161,10 +170,10 @@
         subs r3, r3, #1
         stmia r0, {r4-r5}
         add r0, r0, r2
-        bne 2b
+        bne %BA02
         ldmfd sp!, {r4-r5,pc}
-        .align 8
-3:
+        ALIGN 8
+03
         ldmia r1, {r4-r5, r12}
         add r1, r1, r2
         ADJ_ALIGN_DOUBLEWORD 2, r4, r5, r12
@@ -172,10 +181,10 @@
         subs r3, r3, #1
         stmia r0, {r4-r5}
         add r0, r0, r2
-        bne 3b
+        bne %BA03
         ldmfd sp!, {r4-r5,pc}
-        .align 8
-4:
+        ALIGN 8
+04
         ldmia r1, {r4-r5, r12}
         add r1, r1, r2
         ADJ_ALIGN_DOUBLEWORD 3, r4, r5, r12
@@ -183,30 +192,30 @@
         subs r3, r3, #1
         stmia r0, {r4-r5}
         add r0, r0, r2
-        bne 4b
+        bne %BA04
         ldmfd sp!, {r4-r5,pc}
-        .align 8
-5:
-        .word 1b
-        .word 2b
-        .word 3b
-        .word 4b
-
-@ ----------------------------------------------------------------
-        .align 8
-        .global put_pixels8_x2_arm
-put_pixels8_x2_arm:
-        @ void func(uint8_t *block, const uint8_t *pixels, int line_size, int h)
-        @ block = word aligned, pixles = unaligned
+        ALIGN 8
+05
+        DCD %BA01
+        DCD %BA02
+        DCD %BA03
+        DCD %BA04
+
+; ----------------------------------------------------------------
+        ALIGN 8
+        EXPORT put_pixels8_x2_arm
+put_pixels8_x2_arm ROUT
+        ; void func(uint8_t *block, const uint8_t *pixels, int line_size, int h)
+        ; block = word aligned, pixles = unaligned
         pld [r1]
-        stmfd sp!, {r4-r10,lr} @ R14 is also called LR
-        adr r5, 5f
+        stmfd sp!, {r4-r10,lr} ; R14 is also called LR
+        adr r5, %FA05
         ands r4, r1, #3
         ldr r12, [r5]
         add r5, r5, r4, lsl #2
         bic r1, r1, #3
         ldrne pc, [r5]
-1:
+01
         ldmia r1, {r4-r5, r10}
         add r1, r1, r2
         ADJ_ALIGN_DOUBLEWORD_D 1, r6, r7, r4, r5, r10
@@ -215,10 +224,10 @@
         subs r3, r3, #1
         stmia r0, {r8-r9}
         add r0, r0, r2
-        bne 1b
+        bne %BA01
         ldmfd sp!, {r4-r10,pc}
-        .align 8
-2:
+        ALIGN 8
+02
         ldmia r1, {r4-r5, r10}
         add r1, r1, r2
         ADJ_ALIGN_DOUBLEWORD_D 1, r6, r7, r4, r5, r10
@@ -228,10 +237,10 @@
         subs r3, r3, #1
         stmia r0, {r4-r5}
         add r0, r0, r2
-        bne 2b
+        bne %BA02
         ldmfd sp!, {r4-r10,pc}
-        .align 8
-3:
+        ALIGN 8
+03
         ldmia r1, {r4-r5, r10}
         add r1, r1, r2
         ADJ_ALIGN_DOUBLEWORD_D 2, r6, r7, r4, r5, r10
@@ -241,10 +250,10 @@
         subs r3, r3, #1
         stmia r0, {r4-r5}
         add r0, r0, r2
-        bne 3b
+        bne %BA03
         ldmfd sp!, {r4-r10,pc}
-        .align 8
-4:
+        ALIGN 8
+04
         ldmia r1, {r4-r5, r10}
         add r1, r1, r2
         ADJ_ALIGN_DOUBLEWORD_D 3, r6, r7, r4, r5, r10
@@ -253,29 +262,29 @@
         subs r3, r3, #1
         stmia r0, {r8-r9}
         add r0, r0, r2
-        bne 4b
-        ldmfd sp!, {r4-r10,pc} @@ update PC with LR content.
-        .align 8
-5:
-        .word 0xFEFEFEFE
-        .word 2b
-        .word 3b
-        .word 4b
-
-        .align 8
-        .global put_no_rnd_pixels8_x2_arm
-put_no_rnd_pixels8_x2_arm:
-        @ void func(uint8_t *block, const uint8_t *pixels, int line_size, int h)
-        @ block = word aligned, pixles = unaligned
+        bne %BA04
+        ldmfd sp!, {r4-r10,pc} ;; update PC with LR content.
+        ALIGN 8
+05
+        DCD 0xFEFEFEFE
+        DCD %BA02
+        DCD %BA03
+        DCD %BA04
+
+        ALIGN 8
+        EXPORT put_no_rnd_pixels8_x2_arm
+put_no_rnd_pixels8_x2_arm ROUT
+        ; void func(uint8_t *block, const uint8_t *pixels, int line_size, int h)
+        ; block = word aligned, pixles = unaligned
         pld [r1]
-        stmfd sp!, {r4-r10,lr} @ R14 is also called LR
-        adr r5, 5f
+        stmfd sp!, {r4-r10,lr} ; R14 is also called LR
+        adr r5, %FA05
         ands r4, r1, #3
         ldr r12, [r5]
         add r5, r5, r4, lsl #2
         bic r1, r1, #3
         ldrne pc, [r5]
-1:
+01
         ldmia r1, {r4-r5, r10}
         add r1, r1, r2
         ADJ_ALIGN_DOUBLEWORD_D 1, r6, r7, r4, r5, r10
@@ -284,10 +293,10 @@
         subs r3, r3, #1
         stmia r0, {r8-r9}
         add r0, r0, r2
-        bne 1b
+        bne %BA01
         ldmfd sp!, {r4-r10,pc}
-        .align 8
-2:
+        ALIGN 8
+02
         ldmia r1, {r4-r5, r10}
         add r1, r1, r2
         ADJ_ALIGN_DOUBLEWORD_D 1, r6, r7, r4, r5, r10
@@ -297,10 +306,10 @@
         subs r3, r3, #1
         stmia r0, {r4-r5}
         add r0, r0, r2
-        bne 2b
+        bne %BA02
         ldmfd sp!, {r4-r10,pc}
-        .align 8
-3:
+        ALIGN 8
+03
         ldmia r1, {r4-r5, r10}
         add r1, r1, r2
         ADJ_ALIGN_DOUBLEWORD_D 2, r6, r7, r4, r5, r10
@@ -310,10 +319,10 @@
         subs r3, r3, #1
         stmia r0, {r4-r5}
         add r0, r0, r2
-        bne 3b
+        bne %BA03
         ldmfd sp!, {r4-r10,pc}
-        .align 8
-4:
+        ALIGN 8
+04
         ldmia r1, {r4-r5, r10}
         add r1, r1, r2
         ADJ_ALIGN_DOUBLEWORD_D 3, r6, r7, r4, r5, r10
@@ -322,35 +331,35 @@
         subs r3, r3, #1
         stmia r0, {r8-r9}
         add r0, r0, r2
-        bne 4b
-        ldmfd sp!, {r4-r10,pc} @@ update PC with LR content.
-        .align 8
-5:
-        .word 0xFEFEFEFE
-        .word 2b
-        .word 3b
-        .word 4b
-
-
-@ ----------------------------------------------------------------
-        .align 8
-        .global put_pixels8_y2_arm
-put_pixels8_y2_arm:
-        @ void func(uint8_t *block, const uint8_t *pixels, int line_size, int h)
-        @ block = word aligned, pixles = unaligned
+        bne %BA04
+        ldmfd sp!, {r4-r10,pc} ;; update PC with LR content.
+        ALIGN 8
+05
+        DCD 0xFEFEFEFE
+        DCD %BA02
+        DCD %BA03
+        DCD %BA04
+
+
+; ----------------------------------------------------------------
+        ALIGN 8
+        EXPORT put_pixels8_y2_arm
+put_pixels8_y2_arm ROUT
+        ; void func(uint8_t *block, const uint8_t *pixels, int line_size, int h)
+        ; block = word aligned, pixles = unaligned
         pld [r1]
-        stmfd sp!, {r4-r11,lr} @ R14 is also called LR
-        adr r5, 5f
+        stmfd sp!, {r4-r11,lr} ; R14 is also called LR
+        adr r5, %FA05
         ands r4, r1, #3
         mov r3, r3, lsr #1
         ldr r12, [r5]
         add r5, r5, r4, lsl #2
         bic r1, r1, #3
         ldrne pc, [r5]
-1:
+01
         ldmia r1, {r4-r5}
         add r1, r1, r2
-6:      ldmia r1, {r6-r7}
+10      ldmia r1, {r6-r7}
         add r1, r1, r2
         pld [r1]
         RND_AVG32 r8, r9, r4, r5, r6, r7, r12
@@ -363,15 +372,15 @@
         subs r3, r3, #1
         stmia r0, {r8-r9}
         add r0, r0, r2
-        bne 6b
+        bne %BA10
         ldmfd sp!, {r4-r11,pc}
-        .align 8
-2:
+        ALIGN 8
+02
         ldmia r1, {r4-r6}
         add r1, r1, r2
         pld [r1]
         ADJ_ALIGN_DOUBLEWORD 1, r4, r5, r6
-6:      ldmia r1, {r7-r9}
+20      ldmia r1, {r7-r9}
         add r1, r1, r2
         pld [r1]
         ADJ_ALIGN_DOUBLEWORD 1, r7, r8, r9
@@ -386,15 +395,15 @@
         RND_AVG32 r10, r11, r7, r8, r4, r5, r12
         stmia r0, {r10-r11}
         add r0, r0, r2
-        bne 6b
+        bne %BA20
         ldmfd sp!, {r4-r11,pc}
-        .align 8
-3:
+        ALIGN 8
+03
         ldmia r1, {r4-r6}
         add r1, r1, r2
         pld [r1]
         ADJ_ALIGN_DOUBLEWORD 2, r4, r5, r6
-6:      ldmia r1, {r7-r9}
+30      ldmia r1, {r7-r9}
         add r1, r1, r2
         pld [r1]
         ADJ_ALIGN_DOUBLEWORD 2, r7, r8, r9
@@ -409,15 +418,15 @@
         RND_AVG32 r10, r11, r7, r8, r4, r5, r12
         stmia r0, {r10-r11}
         add r0, r0, r2
-        bne 6b
+        bne %BA30
         ldmfd sp!, {r4-r11,pc}
-        .align 8
-4:
+        ALIGN 8
+04
         ldmia r1, {r4-r6}
         add r1, r1, r2
         pld [r1]
         ADJ_ALIGN_DOUBLEWORD 3, r4, r5, r6
-6:      ldmia r1, {r7-r9}
+40      ldmia r1, {r7-r9}
         add r1, r1, r2
         pld [r1]
         ADJ_ALIGN_DOUBLEWORD 3, r7, r8, r9
@@ -432,34 +441,34 @@
         RND_AVG32 r10, r11, r7, r8, r4, r5, r12
         stmia r0, {r10-r11}
         add r0, r0, r2
-        bne 6b
+        bne %BA40
         ldmfd sp!, {r4-r11,pc}

-        .align 8
-5:
-        .word 0xFEFEFEFE
-        .word 2b
-        .word 3b
-        .word 4b
-
-        .align 8
-        .global put_no_rnd_pixels8_y2_arm
-put_no_rnd_pixels8_y2_arm:
-        @ void func(uint8_t *block, const uint8_t *pixels, int line_size, int h)
-        @ block = word aligned, pixles = unaligned
+        ALIGN 8
+05
+        DCD 0xFEFEFEFE
+        DCD %BA02
+        DCD %BA03
+        DCD %BA04
+
+        ALIGN 8
+        EXPORT put_no_rnd_pixels8_y2_arm
+put_no_rnd_pixels8_y2_arm ROUT
+        ; void func(uint8_t *block, const uint8_t *pixels, int line_size, int h)
+        ; block = word aligned, pixles = unaligned
         pld [r1]
-        stmfd sp!, {r4-r11,lr} @ R14 is also called LR
-        adr r5, 5f
+        stmfd sp!, {r4-r11,lr} ; R14 is also called LR
+        adr r5, %FA05
         ands r4, r1, #3
         mov r3, r3, lsr #1
         ldr r12, [r5]
         add r5, r5, r4, lsl #2
         bic r1, r1, #3
         ldrne pc, [r5]
-1:
+01
         ldmia r1, {r4-r5}
         add r1, r1, r2
-6:      ldmia r1, {r6-r7}
+10      ldmia r1, {r6-r7}
         add r1, r1, r2
         pld [r1]
         NO_RND_AVG32 r8, r9, r4, r5, r6, r7, r12
@@ -472,15 +481,15 @@
         subs r3, r3, #1
         stmia r0, {r8-r9}
         add r0, r0, r2
-        bne 6b
+        bne %BA10
         ldmfd sp!, {r4-r11,pc}
-        .align 8
-2:
+        ALIGN 8
+02
         ldmia r1, {r4-r6}
         add r1, r1, r2
         pld [r1]
         ADJ_ALIGN_DOUBLEWORD 1, r4, r5, r6
-6:      ldmia r1, {r7-r9}
+20      ldmia r1, {r7-r9}
         add r1, r1, r2
         pld [r1]
         ADJ_ALIGN_DOUBLEWORD 1, r7, r8, r9
@@ -495,15 +504,15 @@
         NO_RND_AVG32 r10, r11, r7, r8, r4, r5, r12
         stmia r0, {r10-r11}
         add r0, r0, r2
-        bne 6b
+        bne %BA20
         ldmfd sp!, {r4-r11,pc}
-        .align 8
-3:
+        ALIGN 8
+03
         ldmia r1, {r4-r6}
         add r1, r1, r2
         pld [r1]
         ADJ_ALIGN_DOUBLEWORD 2, r4, r5, r6
-6:      ldmia r1, {r7-r9}
+30      ldmia r1, {r7-r9}
         add r1, r1, r2
         pld [r1]
         ADJ_ALIGN_DOUBLEWORD 2, r7, r8, r9
@@ -518,15 +527,15 @@
         NO_RND_AVG32 r10, r11, r7, r8, r4, r5, r12
         stmia r0, {r10-r11}
         add r0, r0, r2
-        bne 6b
+        bne %BA30
         ldmfd sp!, {r4-r11,pc}
-        .align 8
-4:
+        ALIGN 8
+04
         ldmia r1, {r4-r6}
         add r1, r1, r2
         pld [r1]
         ADJ_ALIGN_DOUBLEWORD 3, r4, r5, r6
-6:      ldmia r1, {r7-r9}
+40      ldmia r1, {r7-r9}
         add r1, r1, r2
         pld [r1]
         ADJ_ALIGN_DOUBLEWORD 3, r7, r8, r9
@@ -541,73 +550,83 @@
         NO_RND_AVG32 r10, r11, r7, r8, r4, r5, r12
         stmia r0, {r10-r11}
         add r0, r0, r2
-        bne 6b
+        bne %BA40
         ldmfd sp!, {r4-r11,pc}
-        .align 8
-5:
-        .word 0xFEFEFEFE
-        .word 2b
-        .word 3b
-        .word 4b
-
-@ ----------------------------------------------------------------
-.macro  RND_XY2_IT align, rnd
-        @ l1=  (a & 0x03030303) + (b & 0x03030303) ?(+ 0x02020202)
-        @ h1= ((a & 0xFCFCFCFCUL) >> 2) + ((b & 0xFCFCFCFCUL) >> 2)
-.if \align == 0
+        ALIGN 8
+05
+        DCD 0xFEFEFEFE
+        DCD %BA02
+        DCD %BA03
+        DCD %BA04
+
+; ----------------------------------------------------------------
+	MACRO
+	RND_XY2_IT $align, $rnd
+        ; l1=  (a & 0x03030303) + (b & 0x03030303) ?(+ 0x02020202)
+        ; h1= ((a & 0xFCFCFCFCUL) >> 2) + ((b & 0xFCFCFCFCUL) >> 2)
+ [ $align = 0
         ldmia r1, {r6-r8}
-.elseif \align == 3
+ |
+   [ $align = 3
         ldmia r1, {r5-r7}
-.else
+   |
         ldmia r1, {r8-r10}
-.endif
+   ]
+ ]
         add r1, r1, r2
         pld [r1]
-.if \align == 0
+ [ $align = 0
         ADJ_ALIGN_DOUBLEWORD_D 1, r4, r5, r6, r7, r8
-.elseif \align == 1
+ |
+   [ $align = 1
         ADJ_ALIGN_DOUBLEWORD_D 1, r4, r5, r8, r9, r10
         ADJ_ALIGN_DOUBLEWORD_D 2, r6, r7, r8, r9, r10
-.elseif \align == 2
+   |
+     [ $align = 2
         ADJ_ALIGN_DOUBLEWORD_D 2, r4, r5, r8, r9, r10
         ADJ_ALIGN_DOUBLEWORD_D 3, r6, r7, r8, r9, r10
-.elseif \align == 3
+     |
+       [ $align = 3
         ADJ_ALIGN_DOUBLEWORD_D 3, r4, r5, r5, r6, r7
-.endif
-        ldr r14, [r12, #0]      @ 0x03030303
+       ]
+     ]
+   ]
+ ]
+        ldr r14, [r12, #0]      ; 0x03030303
         tst r3, #1
         and r8, r4, r14
         and r9, r5, r14
         and r10, r6, r14
         and r11, r7, r14
-.if \rnd == 1
-        ldreq r14, [r12, #16]   @ 0x02020202
-.else
-        ldreq r14, [r12, #28]   @ 0x01010101
-.endif
+ [ $rnd = 1
+        ldreq r14, [r12, #16]   ; 0x02020202
+ |
+       ldreq r14, [r12, #28]   ; 0x01010101
+ ]
         add r8, r8, r10
         add r9, r9, r11
         addeq r8, r8, r14
         addeq r9, r9, r14
-        ldr r14, [r12, #20]     @ 0xFCFCFCFC >> 2
+        ldr r14, [r12, #20]     ; 0xFCFCFCFC >> 2
         and r4, r14, r4, lsr #2
         and r5, r14, r5, lsr #2
         and r6, r14, r6, lsr #2
         and r7, r14, r7, lsr #2
         add r10, r4, r6
         add r11, r5, r7
-.endm
+	MEND

-.macro RND_XY2_EXPAND align, rnd
-        RND_XY2_IT \align, \rnd
-6:      stmfd sp!, {r8-r11}
-        RND_XY2_IT \align, \rnd
+        MACRO
+	RND_XY2_EXPAND $align, $rnd
+        RND_XY2_IT $align, $rnd
+00      stmfd sp!, {r8-r11}
+        RND_XY2_IT $align, $rnd
         ldmfd sp!, {r4-r7}
         add r4, r4, r8
         add r5, r5, r9
         add r6, r6, r10
         add r7, r7, r11
-        ldr r14, [r12, #24]     @ 0x0F0F0F0F
+        ldr r14, [r12, #24]     ; 0x0F0F0F0F
         and r4, r14, r4, lsr #2
         and r5, r14, r5, lsr #2
         add r4, r4, r6
@@ -615,80 +634,82 @@
         subs r3, r3, #1
         stmia r0, {r4-r5}
         add r0, r0, r2
-        bne 6b
+        bne %BA00
         ldmfd sp!, {r4-r11,pc}
-.endm
+	MEND

-        .align 8
-        .global put_pixels8_xy2_arm
-put_pixels8_xy2_arm:
-        @ void func(uint8_t *block, const uint8_t *pixels, int line_size, int h)
-        @ block = word aligned, pixles = unaligned
+        ALIGN 8
+        EXPORT put_pixels8_xy2_arm
+put_pixels8_xy2_arm ROUT
+        ; void func(uint8_t *block, const uint8_t *pixels, int line_size, int h)
+        ; block = word aligned, pixles = unaligned
         pld [r1]
-        stmfd sp!, {r4-r11,lr} @ R14 is also called LR
-        adrl r12, 5f
+        stmfd sp!, {r4-r11,lr} ; R14 is also called LR
+        adrl r12, %FA05
         ands r4, r1, #3
         add r5, r12, r4, lsl #2
         bic r1, r1, #3
         ldrne pc, [r5]
-1:
+01
         RND_XY2_EXPAND 0, 1

-        .align 8
-2:
+        ALIGN 8
+02
         RND_XY2_EXPAND 1, 1
-
-        .align 8
-3:
+
+        ALIGN 8
+03
         RND_XY2_EXPAND 2, 1
-
-        .align 8
-4:
+
+        ALIGN 8
+04
         RND_XY2_EXPAND 3, 1
-
-5:
-        .word 0x03030303
-        .word 2b
-        .word 3b
-        .word 4b
-        .word 0x02020202
-        .word 0xFCFCFCFC >> 2
-        .word 0x0F0F0F0F
-        .word 0x01010101
-
-        .align 8
-        .global put_no_rnd_pixels8_xy2_arm
-put_no_rnd_pixels8_xy2_arm:
-        @ void func(uint8_t *block, const uint8_t *pixels, int line_size, int h)
-        @ block = word aligned, pixles = unaligned
+
+05
+        DCD 0x03030303
+        DCD %BA02
+        DCD %BA03
+        DCD %BA04
+        DCD 0x02020202
+        DCD 0xFCFCFCFC >> 2
+        DCD 0x0F0F0F0F
+        DCD 0x01010101
+
+        ALIGN 8
+        EXPORT put_no_rnd_pixels8_xy2_arm
+put_no_rnd_pixels8_xy2_arm ROUT
+        ; void func(uint8_t *block, const uint8_t *pixels, int line_size, int h)
+        ; block = word aligned, pixles = unaligned
         pld [r1]
-        stmfd sp!, {r4-r11,lr} @ R14 is also called LR
-        adrl r12, 5f
+        stmfd sp!, {r4-r11,lr} ; R14 is also called LR
+        adrl r12, %FA05
         ands r4, r1, #3
         add r5, r12, r4, lsl #2
         bic r1, r1, #3
         ldrne pc, [r5]
-1:
+01
         RND_XY2_EXPAND 0, 0

-        .align 8
-2:
+        ALIGN 8
+02
         RND_XY2_EXPAND 1, 0
-
-        .align 8
-3:
+
+        ALIGN 8
+03
         RND_XY2_EXPAND 2, 0
-
-        .align 8
-4:
+
+        ALIGN 8
+04
         RND_XY2_EXPAND 3, 0
-
-5:
-        .word 0x03030303
-        .word 2b
-        .word 3b
-        .word 4b
-        .word 0x02020202
-        .word 0xFCFCFCFC >> 2
-        .word 0x0F0F0F0F
-        .word 0x01010101
+
+05
+        DCD 0x03030303
+        DCD %BA02
+        DCD %BA03
+        DCD %BA04
+        DCD 0x02020202
+        DCD 0xFCFCFCFC >> 2
+        DCD 0x0F0F0F0F
+        DCD 0x01010101
+
+	END
Index: libavcodec/armv4l/jrevdct_arm.S
===================================================================
RCS file: /cvsroot/ffmpeg/ffmpeg/libavcodec/armv4l/jrevdct_arm.S,v
retrieving revision 1.1
diff -u -r1.1 jrevdct_arm.S
--- libavcodec/armv4l/jrevdct_arm.S	13 Aug 2001 21:38:25 -0000	1.1
+++ libavcodec/armv4l/jrevdct_arm.S	21 Jul 2005 04:57:29 -0000
@@ -1,75 +1,94 @@
-/*
-   C-like prototype :
-	void j_rev_dct_ARM(DCTBLOCK data)
-
-   With DCTBLOCK being a pointer to an array of 64 'signed shorts'
-
-   Copyright (c) 2001 Lionel Ulmer (lionel.ulmer@free.fr / bbrox@bbrox.org)
-
-   Permission is hereby granted, free of charge, to any person obtaining a copy
-   of this software and associated documentation files (the "Software"), to deal
-   in the Software without restriction, including without limitation the rights
-   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
-   copies of the Software, and to permit persons to whom the Software is
-   furnished to do so, subject to the following conditions:
-
-   The above copyright notice and this permission notice shall be included in
-   all copies or substantial portions of the Software.
-
-   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
-   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
-   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
-   COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
-   IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
-   CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
-
-*/
-#define FIX_0_298631336 2446
-#define FIX_0_541196100 4433
-#define FIX_0_765366865 6270
-#define FIX_1_175875602 9633
-#define FIX_1_501321110 12299
-#define FIX_2_053119869 16819
-#define FIX_3_072711026 25172
-#define FIX_M_0_390180644 -3196
-#define FIX_M_0_899976223 -7373
-#define FIX_M_1_847759065 -15137
-#define FIX_M_1_961570560 -16069
-#define FIX_M_2_562915447 -20995
-#define FIX_0xFFFF 0xFFFF
-
-#define FIX_0_298631336_ID      0
-#define FIX_0_541196100_ID      4
-#define FIX_0_765366865_ID      8
-#define FIX_1_175875602_ID     12
-#define FIX_1_501321110_ID     16
-#define FIX_2_053119869_ID     20
-#define FIX_3_072711026_ID     24
-#define FIX_M_0_390180644_ID   28
-#define FIX_M_0_899976223_ID   32
-#define FIX_M_1_847759065_ID   36
-#define FIX_M_1_961570560_ID   40
-#define FIX_M_2_562915447_ID   44
-#define FIX_0xFFFF_ID          48
-	.text
-	.align
-
-	.global j_rev_dct_ARM
-j_rev_dct_ARM:
-	stmdb   sp!, { r4 - r12, lr }   @ all callee saved regs
-
-	sub sp, sp, #4                  @ reserve some space on the stack
-	str r0, [ sp ]                  @ save the DCT pointer to the stack
-
-	mov lr, r0                      @ lr = pointer to the current row
-	mov r12, #8                     @ r12 = row-counter
-	add r11, pc, #(const_array-.-8) @ r11 = base pointer to the constants array
-row_loop:
-	ldrsh r0, [lr, # 0]             @ r0 = 'd0'
-	ldrsh r1, [lr, # 8]             @ r1 = 'd1'
+;
+;   C-like prototype :
+;	void j_rev_dct_ARM(DCTBLOCK data)
+;
+;   With DCTBLOCK being a pointer to an array of 64 'signed shorts'
+;
+;   Copyright (c) 2001 Lionel Ulmer (lionel.ulmer@free.fr / bbrox@bbrox.org)
+;
+;   Permission is hereby granted, free of charge, to any person obtaining a copy
+;   of this software and associated documentation files (the "Software"), to deal
+;   in the Software without restriction, including without limitation the rights
+;   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
+;   copies of the Software, and to permit persons to whom the Software is
+;   furnished to do so, subject to the following conditions:
+;
+;   The above copyright notice and this permission notice shall be included in
+;   all copies or substantial portions of the Software.
+;
+;   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
+;   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
+;   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
+;   COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
+;   IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
+;   CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
+;
+;
+FIX_0_298631336		*	2446
+FIX_0_541196100		*	4433
+FIX_0_765366865		*	6270
+FIX_1_175875602		*	9633
+FIX_1_501321110		*	12299
+FIX_2_053119869		*	16819
+FIX_3_072711026		*	25172
+FIX_M_0_390180644	*	-3196
+FIX_M_0_899976223	*	-7373
+FIX_M_1_847759065	*	-15137
+FIX_M_1_961570560	*	-16069
+FIX_M_2_562915447	*	-20995
+FIX_0xFFFF		*	0xFFFF
+
+FIX_0_298631336_ID	*	 0
+FIX_0_541196100_ID	*	 4
+FIX_0_765366865_ID	*	 8
+FIX_1_175875602_ID	*	12
+FIX_1_501321110_ID	*	16
+FIX_2_053119869_ID	*	20
+FIX_3_072711026_ID	*	24
+FIX_M_0_390180644_ID	*	28
+FIX_M_0_899976223_ID	*	32
+FIX_M_1_847759065_ID	*	36
+FIX_M_1_961570560_ID	*	40
+FIX_M_2_562915447_ID	*	44
+FIX_0xFFFF_ID		*	48
+
+	GBLL	NO_HALFWORD
+	GBLL	HAVE_ADRL
+NO_HALFWORD	SETL	{TRUE}
+HAVE_ADRL	SETL	{TRUE}
+
+	AREA |C$$Code|, CODE, READONLY
+	ALIGN
+
+	EXPORT j_rev_dct_ARM
+j_rev_dct_ARM ROUT
+	stmdb   sp!, { r4 - r12, lr }   ; all callee saved regs
+
+	sub sp, sp, #4                  ; reserve some space on the stack
+	str r0, [ sp ]                  ; save the DCT pointer to the stack
+
+	mov lr, r0                      ; lr = pointer to the current row
+	mov r12, #8                     ; r12 = row-counter
+ [ HAVE_ADRL
+	adrl r11, const_array
+ |
+	add r11, pc, #(const_array-.-8) ; r11 = base pointer to the constants array
+ ]
+row_loop
+ [ NO_HALFWORD
+	ldr r0, [lr, #0]
+	mov r0, r0, LSL #16
+	mov r0, r0, ASR #16
+	ldr r1, [lr, #8]
+	mov r1, r1, LSL #16
+	mov r1, r1, ASR #16
+ |
+	ldrsh r0, [lr, # 0]             ; r0 = 'd0'
+	ldrsh r1, [lr, # 8]             ; r1 = 'd1'
+ ]

-	@ Optimization for row that have all items except the first set to 0
-	@ (this works as the DCTELEMS are always 4-byte aligned)
+	; Optimization for row that have all items except the first set to 0
+	; (this works as the DCTELEMS are always 4-byte aligned)
 	ldr r5, [lr, # 0]
 	ldr r2, [lr, # 4]
 	ldr r3, [lr, # 8]
@@ -77,121 +96,187 @@
 	orr r3, r3, r4
 	orr r3, r3, r2
 	orrs r5, r3, r5
-	beq end_of_row_loop             @ nothing to be done as ALL of them are '0'
+	beq end_of_row_loop             ; nothing to be done as ALL of them are '0'
 	orrs r2, r3, r1
 	beq empty_row
-
-	ldrsh r2, [lr, # 2]             @ r2 = 'd2'
-	ldrsh r4, [lr, # 4]             @ r4 = 'd4'
-	ldrsh r6, [lr, # 6]             @ r6 = 'd6'
-
+
+ [ NO_HALFWORD
+	ldr r2, [lr, #0]
+	mov r2, r2, ASR #16
+	ldr r4, [lr, #4]
+	mov r6, r4, ASR #16
+	mov r4, r4, LSL #16
+	mov r4, r4, ASR #16
+ |
+	ldrsh r2, [lr, # 2]             ; r2 = 'd2'
+	ldrsh r4, [lr, # 4]             ; r4 = 'd4'
+	ldrsh r6, [lr, # 6]             ; r6 = 'd6'
+ ]
+
 	ldr r3, [r11, #FIX_0_541196100_ID]
 	add r7, r2, r6
 	ldr r5, [r11, #FIX_M_1_847759065_ID]
-	mul r7, r3, r7                      @ r7 = z1
+	mul r7, r3, r7                      ; r7 = z1
 	ldr r3, [r11, #FIX_0_765366865_ID]
-	mla r6, r5, r6, r7                  @ r6 = tmp2
-	add r5, r0, r4                      @ r5 = tmp0
-	mla r2, r3, r2, r7                  @ r2 = tmp3
-	sub r3, r0, r4                      @ r3 = tmp1
-
-	add r0, r2, r5, lsl #13             @ r0 = tmp10
-	rsb r2, r2, r5, lsl #13             @ r2 = tmp13
-	add r4, r6, r3, lsl #13             @ r4 = tmp11
-	rsb r3, r6, r3, lsl #13             @ r3 = tmp12
-
-	stmdb   sp!, { r0, r2, r3, r4 } @ save on the stack tmp10, tmp13, tmp12, tmp11
-
-	ldrsh r3, [lr, #10]             @ r3 = 'd3'
-	ldrsh r5, [lr, #12]             @ r5 = 'd5'
-	ldrsh r7, [lr, #14]             @ r7 = 'd7'
-
-	add r0, r3, r5	                @ r0 = 'z2'
-	add r2, r1, r7                  @ r2 = 'z1'
-	add r4, r3, r7                  @ r4 = 'z3'
-	add r6, r1, r5                  @ r6 = 'z4'
+	mla r6, r5, r6, r7                  ; r6 = tmp2
+	add r5, r0, r4                      ; r5 = tmp0
+	mla r2, r3, r2, r7                  ; r2 = tmp3
+	sub r3, r0, r4                      ; r3 = tmp1
+
+	add r0, r2, r5, lsl #13             ; r0 = tmp10
+	rsb r2, r2, r5, lsl #13             ; r2 = tmp13
+	add r4, r6, r3, lsl #13             ; r4 = tmp11
+	rsb r3, r6, r3, lsl #13             ; r3 = tmp12
+
+	stmdb   sp!, { r0, r2, r3, r4 } ; save on the stack tmp10, tmp13, tmp12, tmp11
+
+ [ NO_HALFWORD
+	ldr r3, [lr, #8]
+	mov r3, r3, ASR #16
+	ldr r5, [lr, #12]
+	mov r7, r5, ASR #16
+	mov r5, r5, LSL #16
+	mov r5, r5, ASR #16
+ |
+	ldrsh r3, [lr, #10]             ; r3 = 'd3'
+	ldrsh r5, [lr, #12]             ; r5 = 'd5'
+	ldrsh r7, [lr, #14]             ; r7 = 'd7'
+ ]
+
+	add r0, r3, r5	                ; r0 = 'z2'
+	add r2, r1, r7                  ; r2 = 'z1'
+	add r4, r3, r7                  ; r4 = 'z3'
+	add r6, r1, r5                  ; r6 = 'z4'
 	ldr r9, [r11, #FIX_1_175875602_ID]
-	add r8, r4, r6                  @ r8 = z3 + z4
+	add r8, r4, r6                  ; r8 = z3 + z4
 	ldr r10, [r11, #FIX_M_0_899976223_ID]
-	mul r8, r9, r8                  @ r8 = 'z5'
+	mul r8, r9, r8                  ; r8 = 'z5'
 	ldr r9, [r11, #FIX_M_2_562915447_ID]
-	mul r2, r10, r2                 @ r2 = 'z1'
+	mul r2, r10, r2                 ; r2 = 'z1'
 	ldr r10, [r11, #FIX_M_1_961570560_ID]
-	mul r0, r9, r0                  @ r0 = 'z2'
+	mul r0, r9, r0                  ; r0 = 'z2'
 	ldr r9, [r11, #FIX_M_0_390180644_ID]
-	mla r4, r10, r4, r8             @ r4 = 'z3'
+	mla r4, r10, r4, r8             ; r4 = 'z3'
 	ldr r10, [r11, #FIX_0_298631336_ID]
-	mla r6, r9, r6, r8              @ r6 = 'z4'
+	mla r6, r9, r6, r8              ; r6 = 'z4'
 	ldr r9, [r11, #FIX_2_053119869_ID]
-	mla r7, r10, r7, r2             @ r7 = tmp0 + z1
+	mla r7, r10, r7, r2             ; r7 = tmp0 + z1
 	ldr r10, [r11, #FIX_3_072711026_ID]
-	mla r5, r9, r5, r0              @ r5 = tmp1 + z2
+	mla r5, r9, r5, r0              ; r5 = tmp1 + z2
 	ldr r9, [r11, #FIX_1_501321110_ID]
-	mla r3, r10, r3, r0             @ r3 = tmp2 + z2
-	add r7, r7, r4                  @ r7 = tmp0
-	mla r1, r9, r1, r2              @ r1 = tmp3 + z1
-	add r5,	r5, r6                  @ r5 = tmp1
-	add r3, r3, r4                  @ r3 = tmp2
-	add r1, r1, r6                  @ r1 = tmp3
-
-	ldmia sp!, { r0, r2, r4, r6 } @ r0 = tmp10 / r2 = tmp13 / r4 = tmp12 / r6 = tmp11
-	                              @ r1 = tmp3  / r3 = tmp2  / r5 = tmp1  / r7 = tmp0
-
-	@ Compute DESCALE(tmp10 + tmp3, CONST_BITS-PASS1_BITS)
+	mla r3, r10, r3, r0             ; r3 = tmp2 + z2
+	add r7, r7, r4                  ; r7 = tmp0
+	mla r1, r9, r1, r2              ; r1 = tmp3 + z1
+	add r5,	r5, r6                  ; r5 = tmp1
+	add r3, r3, r4                  ; r3 = tmp2
+	add r1, r1, r6                  ; r1 = tmp3
+
+	ldmia sp!, { r0, r2, r4, r6 } ; r0 = tmp10 / r2 = tmp13 / r4 = tmp12 / r6 = tmp11
+	                              ; r1 = tmp3  / r3 = tmp2  / r5 = tmp1  / r7 = tmp0
+
+	; Compute DESCALE(tmp10 + tmp3, CONST_BITS-PASS1_BITS)
 	add r8, r0, r1
 	add r8, r8, #(1<<10)
 	mov r8, r8, asr #11
+ [ NO_HALFWORD
+	strb r8, [lr, #0]
+	mov r8, r8, LSR #8
+	strb r8, [lr, #1]
+ |
 	strh r8, [lr, # 0]
-
-	@ Compute DESCALE(tmp10 - tmp3, CONST_BITS-PASS1_BITS)
+ ]
+
+	; Compute DESCALE(tmp10 - tmp3, CONST_BITS-PASS1_BITS)
 	sub r8, r0, r1
 	add r8, r8, #(1<<10)
 	mov r8, r8, asr #11
+ [ NO_HALFWORD
+	strb r8, [lr, #14]
+	mov r8, r8, LSR #8
+	strb r8, [lr, #15]
+ |
 	strh r8, [lr, #14]
-
-	@ Compute DESCALE(tmp11 + tmp2, CONST_BITS-PASS1_BITS)
+ ]
+
+	; Compute DESCALE(tmp11 + tmp2, CONST_BITS-PASS1_BITS)
 	add r8, r6, r3
 	add r8, r8, #(1<<10)
 	mov r8, r8, asr #11
+ [ NO_HALFWORD
+	strb r8, [lr, #2]
+	mov r8, r8, LSR #8
+	strb r8, [lr, #3]
+ |
 	strh r8, [lr, # 2]
-
-	@ Compute DESCALE(tmp11 - tmp2, CONST_BITS-PASS1_BITS)
+ ]
+
+	; Compute DESCALE(tmp11 - tmp2, CONST_BITS-PASS1_BITS)
 	sub r8, r6, r3
 	add r8, r8, #(1<<10)
 	mov r8, r8, asr #11
+ [ NO_HALFWORD
+	strb r8, [lr, #12]
+	mov r8, r8, LSR #8
+	strb r8, [lr, #13]
+ |
 	strh r8, [lr, #12]
-
-	@ Compute DESCALE(tmp12 + tmp1, CONST_BITS-PASS1_BITS)
+ ]
+
+	; Compute DESCALE(tmp12 + tmp1, CONST_BITS-PASS1_BITS)
 	add r8, r4, r5
 	add r8, r8, #(1<<10)
 	mov r8, r8, asr #11
+ [ NO_HALFWORD
+	strb r8, [lr, #4]
+	mov r8, r8, LSR #8
+	strb r8, [lr, #5]
+ |
 	strh r8, [lr, # 4]
-
-	@ Compute DESCALE(tmp12 - tmp1, CONST_BITS-PASS1_BITS)
+ ]
+
+	; Compute DESCALE(tmp12 - tmp1, CONST_BITS-PASS1_BITS)
 	sub r8, r4, r5
 	add r8, r8, #(1<<10)
 	mov r8, r8, asr #11
+ [ NO_HALFWORD
+	strb r8, [lr, #10]
+	mov r8, r8, LSR #8
+	strb r8, [lr, #11]
+ |
 	strh r8, [lr, #10]
-
-	@ Compute DESCALE(tmp13 + tmp0, CONST_BITS-PASS1_BITS)
+ ]
+
+	; Compute DESCALE(tmp13 + tmp0, CONST_BITS-PASS1_BITS)
 	add r8, r2, r7
 	add r8, r8, #(1<<10)
 	mov r8, r8, asr #11
+ [ NO_HALFWORD
+	strb r8, [lr, #6]
+	mov r8, r8, LSR #8
+	strb r8, [lr, #7]
+ |
 	strh r8, [lr, # 6]
-
-	@ Compute DESCALE(tmp13 - tmp0, CONST_BITS-PASS1_BITS)
+ ]
+
+	; Compute DESCALE(tmp13 - tmp0, CONST_BITS-PASS1_BITS)
 	sub r8, r2, r7
 	add r8, r8, #(1<<10)
 	mov r8, r8, asr #11
+ [ NO_HALFWORD
+	strb r8, [lr, #8]
+	mov r8, r8, LSR #8
+	strb r8, [lr, #9]
+ |
 	strh r8, [lr, # 8]
+ ]

-	@ End of row loop
+	; End of row loop
 	add lr, lr, #16
 	subs r12, r12, #1
 	bne row_loop
 	beq start_column_loop
-
-empty_row:
+
+empty_row
 	ldr r1, [r11, #FIX_0xFFFF_ID]
 	mov r0, r0, lsl #2
 	and r0, r0, r1
@@ -201,186 +286,298 @@
 	str r0, [lr, # 8]
 	str r0, [lr, #12]

-end_of_row_loop:
-	@ End of loop
+end_of_row_loop
+	; End of loop
 	add lr, lr, #16
 	subs r12, r12, #1
 	bne row_loop

-start_column_loop:
-	@ Start of column loop
+start_column_loop
+	; Start of column loop
 	ldr lr, [ sp ]
 	mov r12, #8
-column_loop:
-	ldrsh r0, [lr, #( 0*8)]             @ r0 = 'd0'
-	ldrsh r2, [lr, #( 4*8)]             @ r2 = 'd2'
-	ldrsh r4, [lr, #( 8*8)]             @ r4 = 'd4'
-	ldrsh r6, [lr, #(12*8)]             @ r6 = 'd6'
+column_loop
+ [ NO_HALFWORD
+	ldr r0, [lr, #0]
+	mov r0, r0, LSL #16
+	mov r0, r0, ASR #16
+	ldr r2, [lr, #32]
+	mov r2, r2, LSL #16
+	mov r2, r2, ASR #16
+	ldr r4, [lr, #64]
+	mov r4, r4, LSL #16
+	mov r4, r4, ASR #16
+	ldr r6, [lr, #96]
+	mov r6, r6, LSL #16
+	mov r6, r6, ASR #16
+ |
+	ldrsh r0, [lr, #( 0*8)]             ; r0 = 'd0'
+	ldrsh r2, [lr, #( 4*8)]             ; r2 = 'd2'
+	ldrsh r4, [lr, #( 8*8)]             ; r4 = 'd4'
+	ldrsh r6, [lr, #(12*8)]             ; r6 = 'd6'
+ ]

 	ldr r3, [r11, #FIX_0_541196100_ID]
 	add r1, r2, r6
 	ldr r5, [r11, #FIX_M_1_847759065_ID]
-	mul r1, r3, r1                      @ r1 = z1
+	mul r1, r3, r1                      ; r1 = z1
 	ldr r3, [r11, #FIX_0_765366865_ID]
-	mla r6, r5, r6, r1                  @ r6 = tmp2
-	add r5, r0, r4                      @ r5 = tmp0
-	mla r2, r3, r2, r1                  @ r2 = tmp3
-	sub r3, r0, r4                      @ r3 = tmp1
-
-	add r0, r2, r5, lsl #13             @ r0 = tmp10
-	rsb r2, r2, r5, lsl #13             @ r2 = tmp13
-	add r4, r6, r3, lsl #13             @ r4 = tmp11
-	rsb r6, r6, r3, lsl #13             @ r6 = tmp12
-
-	ldrsh r1, [lr, #( 2*8)]             @ r1 = 'd1'
-	ldrsh r3, [lr, #( 6*8)]             @ r3 = 'd3'
-	ldrsh r5, [lr, #(10*8)]             @ r5 = 'd5'
-	ldrsh r7, [lr, #(14*8)]             @ r7 = 'd7'
+	mla r6, r5, r6, r1                  ; r6 = tmp2
+	add r5, r0, r4                      ; r5 = tmp0
+	mla r2, r3, r2, r1                  ; r2 = tmp3
+	sub r3, r0, r4                      ; r3 = tmp1
+
+	add r0, r2, r5, lsl #13             ; r0 = tmp10
+	rsb r2, r2, r5, lsl #13             ; r2 = tmp13
+	add r4, r6, r3, lsl #13             ; r4 = tmp11
+	rsb r6, r6, r3, lsl #13             ; r6 = tmp12
+
+ [ NO_HALFWORD
+	ldr r1, [lr, #16]
+	mov r1, r1, LSL #16
+	mov r1, r1, ASR #16
+	ldr r3, [lr, #48]
+	mov r3, r3, LSL #16
+	mov r3, r3, ASR #16
+	ldr r5, [lr, #80]
+	mov r5, r5, LSL #16
+	mov r5, r5, ASR #16
+	ldr r7, [lr, #112]
+	mov r7, r7, LSL #16
+	mov r7, r7, ASR #16
+ |
+	ldrsh r1, [lr, #( 2*8)]             ; r1 = 'd1'
+	ldrsh r3, [lr, #( 6*8)]             ; r3 = 'd3'
+	ldrsh r5, [lr, #(10*8)]             ; r5 = 'd5'
+	ldrsh r7, [lr, #(14*8)]             ; r7 = 'd7'
+ ]

-	@ Check for empty odd column (happens about 20 to 25 % of the time according to my stats)
+	; Check for empty odd column (happens about 20 to 25 % of the time according to my stats)
 	orr r9, r1, r3
 	orr r10, r5, r7
 	orrs r10, r9, r10
 	beq empty_odd_column

-	stmdb   sp!, { r0, r2, r4, r6 } @ save on the stack tmp10, tmp13, tmp12, tmp11
-
-	add r0, r3, r5	                @ r0 = 'z2'
-	add r2, r1, r7                  @ r2 = 'z1'
-	add r4, r3, r7                  @ r4 = 'z3'
-	add r6, r1, r5                  @ r6 = 'z4'
+	stmdb   sp!, { r0, r2, r4, r6 } ; save on the stack tmp10, tmp13, tmp12, tmp11
+
+	add r0, r3, r5	                ; r0 = 'z2'
+	add r2, r1, r7                  ; r2 = 'z1'
+	add r4, r3, r7                  ; r4 = 'z3'
+	add r6, r1, r5                  ; r6 = 'z4'
 	ldr r9, [r11, #FIX_1_175875602_ID]
 	add r8, r4, r6
 	ldr r10, [r11, #FIX_M_0_899976223_ID]
-	mul r8, r9, r8                  @ r8 = 'z5'
+	mul r8, r9, r8                  ; r8 = 'z5'
 	ldr r9, [r11, #FIX_M_2_562915447_ID]
-	mul r2, r10, r2                 @ r2 = 'z1'
+	mul r2, r10, r2                 ; r2 = 'z1'
 	ldr r10, [r11, #FIX_M_1_961570560_ID]
-	mul r0, r9, r0                  @ r0 = 'z2'
+	mul r0, r9, r0                  ; r0 = 'z2'
 	ldr r9, [r11, #FIX_M_0_390180644_ID]
-	mla r4, r10, r4, r8             @ r4 = 'z3'
+	mla r4, r10, r4, r8             ; r4 = 'z3'
 	ldr r10, [r11, #FIX_0_298631336_ID]
-	mla r6, r9, r6, r8              @ r6 = 'z4'
+	mla r6, r9, r6, r8              ; r6 = 'z4'
 	ldr r9, [r11, #FIX_2_053119869_ID]
-	mla r7, r10, r7, r2             @ r7 = tmp0 + z1
+	mla r7, r10, r7, r2             ; r7 = tmp0 + z1
 	ldr r10, [r11, #FIX_3_072711026_ID]
-	mla r5, r9, r5, r0              @ r5 = tmp1 + z2
+	mla r5, r9, r5, r0              ; r5 = tmp1 + z2
 	ldr r9, [r11, #FIX_1_501321110_ID]
-	mla r3, r10, r3, r0             @ r3 = tmp2 + z2
-	add r7, r7, r4                  @ r7 = tmp0
-	mla r1, r9, r1, r2              @ r1 = tmp3 + z1
-	add r5,	r5, r6                  @ r5 = tmp1
-	add r3, r3, r4                  @ r3 = tmp2
-	add r1, r1, r6                  @ r1 = tmp3
-
-	ldmia sp!, { r0, r2, r4, r6 } @ r0 = tmp10 / r2 = tmp13 / r4 = tmp11 / r6 = tmp12
-	                              @ r1 = tmp3  / r3 = tmp2  / r5 = tmp1  / r7 = tmp0
+	mla r3, r10, r3, r0             ; r3 = tmp2 + z2
+	add r7, r7, r4                  ; r7 = tmp0
+	mla r1, r9, r1, r2              ; r1 = tmp3 + z1
+	add r5,	r5, r6                  ; r5 = tmp1
+	add r3, r3, r4                  ; r3 = tmp2
+	add r1, r1, r6                  ; r1 = tmp3
+
+	ldmia sp!, { r0, r2, r4, r6 } ; r0 = tmp10 / r2 = tmp13 / r4 = tmp11 / r6 = tmp12
+	                              ; r1 = tmp3  / r3 = tmp2  / r5 = tmp1  / r7 = tmp0

-	@ Compute DESCALE(tmp10 + tmp3, CONST_BITS+PASS1_BITS+3)
+	; Compute DESCALE(tmp10 + tmp3, CONST_BITS+PASS1_BITS+3)
 	add r8, r0, r1
 	add r8, r8, #(1<<17)
 	mov r8, r8, asr #18
+ [ NO_HALFWORD
+	strb r8, [lr, #0]
+	mov r8, r8, LSR #8
+	strb r8, [lr, #1]
+ |
 	strh r8, [lr, #( 0*8)]
-
-	@ Compute DESCALE(tmp10 - tmp3, CONST_BITS+PASS1_BITS+3)
+ ]
+
+	; Compute DESCALE(tmp10 - tmp3, CONST_BITS+PASS1_BITS+3)
 	sub r8, r0, r1
 	add r8, r8, #(1<<17)
 	mov r8, r8, asr #18
+ [ NO_HALFWORD
+	strb r8, [lr, #112]
+	mov r8, r8, LSR #8
+	strb r8, [lr, #113]
+ |
 	strh r8, [lr, #(14*8)]
-
-	@ Compute DESCALE(tmp11 + tmp2, CONST_BITS+PASS1_BITS+3)
+ ]
+
+	; Compute DESCALE(tmp11 + tmp2, CONST_BITS+PASS1_BITS+3)
 	add r8, r4, r3
 	add r8, r8, #(1<<17)
 	mov r8, r8, asr #18
+ [ NO_HALFWORD
+	strb r8, [lr, #16]
+	mov r8, r8, LSR #8
+	strb r8, [lr, #17]
+ |
 	strh r8, [lr, #( 2*8)]
-
-	@ Compute DESCALE(tmp11 - tmp2, CONST_BITS+PASS1_BITS+3)
+ ]
+
+	; Compute DESCALE(tmp11 - tmp2, CONST_BITS+PASS1_BITS+3)
 	sub r8, r4, r3
 	add r8, r8, #(1<<17)
 	mov r8, r8, asr #18
+ [ NO_HALFWORD
+	strb r8, [lr, #96]
+	mov r8, r8, LSR #8
+	strb r8, [lr, #97]
+ |
 	strh r8, [lr, #(12*8)]
-
-	@ Compute DESCALE(tmp12 + tmp1, CONST_BITS+PASS1_BITS+3)
+ ]
+
+	; Compute DESCALE(tmp12 + tmp1, CONST_BITS+PASS1_BITS+3)
 	add r8, r6, r5
 	add r8, r8, #(1<<17)
 	mov r8, r8, asr #18
+ [ NO_HALFWORD
+	strb r8, [lr, #32]
+	mov r8, r8, LSR #8
+	strb r8, [lr, #33]
+ |
 	strh r8, [lr, #( 4*8)]
-
-	@ Compute DESCALE(tmp12 - tmp1, CONST_BITS+PASS1_BITS+3)
+ ]
+
+	; Compute DESCALE(tmp12 - tmp1, CONST_BITS+PASS1_BITS+3)
 	sub r8, r6, r5
 	add r8, r8, #(1<<17)
 	mov r8, r8, asr #18
+ [ NO_HALFWORD
+	strb r8, [lr, #80]
+	mov r8, r8, LSR #8
+	strb r8, [lr, #81]
+ |
 	strh r8, [lr, #(10*8)]
-
-	@ Compute DESCALE(tmp13 + tmp0, CONST_BITS+PASS1_BITS+3)
+ ]
+
+	; Compute DESCALE(tmp13 + tmp0, CONST_BITS+PASS1_BITS+3)
 	add r8, r2, r7
 	add r8, r8, #(1<<17)
 	mov r8, r8, asr #18
+ [ NO_HALFWORD
+	strb r8, [lr, #48]
+	mov r8, r8, LSR #8
+	strb r8, [lr, #49]
+ |
 	strh r8, [lr, #( 6*8)]
-
-	@ Compute DESCALE(tmp13 - tmp0, CONST_BITS+PASS1_BITS+3)
+ ]
+
+	; Compute DESCALE(tmp13 - tmp0, CONST_BITS+PASS1_BITS+3)
 	sub r8, r2, r7
 	add r8, r8, #(1<<17)
 	mov r8, r8, asr #18
+ [ NO_HALFWORD
+	strb r8, [lr, #64]
+	mov r8, r8, LSR #8
+	strb r8, [lr, #65]
+ |
 	strh r8, [lr, #( 8*8)]
+ ]

-	@ End of row loop
+	; End of row loop
 	add lr, lr, #2
 	subs r12, r12, #1
 	bne column_loop
 	beq the_end
-
-empty_odd_column:
-	@ Compute DESCALE(tmp10 + tmp3, CONST_BITS+PASS1_BITS+3)
-	@ Compute DESCALE(tmp10 - tmp3, CONST_BITS+PASS1_BITS+3)
+
+empty_odd_column
+	; Compute DESCALE(tmp10 + tmp3, CONST_BITS+PASS1_BITS+3)
+	; Compute DESCALE(tmp10 - tmp3, CONST_BITS+PASS1_BITS+3)
 	add r0, r0, #(1<<17)
 	mov r0, r0, asr #18
+ [ NO_HALFWORD
+	strb r0, [lr, #0]
+	strb r0, [lr, #112]
+	mov r0, r0, LSR #8
+	strb r0, [lr, #1]
+  	strb r0, [lr, #113]
+ |
 	strh r0, [lr, #( 0*8)]
 	strh r0, [lr, #(14*8)]
-
-	@ Compute DESCALE(tmp11 + tmp2, CONST_BITS+PASS1_BITS+3)
-	@ Compute DESCALE(tmp11 - tmp2, CONST_BITS+PASS1_BITS+3)
+ ]
+
+	; Compute DESCALE(tmp11 + tmp2, CONST_BITS+PASS1_BITS+3)
+	; Compute DESCALE(tmp11 - tmp2, CONST_BITS+PASS1_BITS+3)
 	add r4, r4, #(1<<17)
 	mov r4, r4, asr #18
+ [ NO_HALFWORD
+	strb r4, [lr, #16]
+	strb r4, [lr, #96]
+	mov r4, r4, LSR #8
+	strb r4, [lr, #17]
+	strb r4, [lr, #97]
+ |
 	strh r4, [lr, #( 2*8)]
 	strh r4, [lr, #(12*8)]
-
-	@ Compute DESCALE(tmp12 + tmp1, CONST_BITS+PASS1_BITS+3)
-	@ Compute DESCALE(tmp12 - tmp1, CONST_BITS+PASS1_BITS+3)
+ ]
+
+	; Compute DESCALE(tmp12 + tmp1, CONST_BITS+PASS1_BITS+3)
+	; Compute DESCALE(tmp12 - tmp1, CONST_BITS+PASS1_BITS+3)
 	add r6, r6, #(1<<17)
 	mov r6, r6, asr #18
+ [ NO_HALFWORD
+	strb r6, [lr, #32]
+	strb r6, [lr, #80]
+	mov r6, r6, LSR #8
+	strb r6, [lr, #33]
+	strb r6, [lr, #81]
+ |
 	strh r6, [lr, #( 4*8)]
 	strh r6, [lr, #(10*8)]
-
-	@ Compute DESCALE(tmp13 + tmp0, CONST_BITS+PASS1_BITS+3)
-	@ Compute DESCALE(tmp13 - tmp0, CONST_BITS+PASS1_BITS+3)
+ ]
+
+	; Compute DESCALE(tmp13 + tmp0, CONST_BITS+PASS1_BITS+3)
+	; Compute DESCALE(tmp13 - tmp0, CONST_BITS+PASS1_BITS+3)
 	add r2, r2, #(1<<17)
 	mov r2, r2, asr #18
+ [ NO_HALFWORD
+	strb r2, [lr, #48]
+	strb r2, [lr, #64]
+	mov r2, r2, LSR #8
+	strb r2, [lr, #49]
+	strb r2, [lr, #65]
+ |
 	strh r2, [lr, #( 6*8)]
 	strh r2, [lr, #( 8*8)]
+ ]

-	@ End of row loop
+	; End of row loop
 	add lr, lr, #2
 	subs r12, r12, #1
 	bne column_loop
-
-the_end:
-	@ The end....
+
+the_end
+	; The end....
 	add sp, sp, #4
-	ldmia   sp!, { r4 - r12, pc }   @ restore callee saved regs and return
+	ldmia   sp!, { r4 - r12, pc }   ; restore callee saved regs and return
+
+const_array
+	ALIGN
+	DCD FIX_0_298631336
+	DCD FIX_0_541196100
+	DCD FIX_0_765366865
+	DCD FIX_1_175875602
+	DCD FIX_1_501321110
+	DCD FIX_2_053119869
+	DCD FIX_3_072711026
+	DCD FIX_M_0_390180644
+	DCD FIX_M_0_899976223
+	DCD FIX_M_1_847759065
+	DCD FIX_M_1_961570560
+	DCD FIX_M_2_562915447
+	DCD FIX_0xFFFF

-const_array:
-	.align
-	.word FIX_0_298631336
-	.word FIX_0_541196100
-	.word FIX_0_765366865
-	.word FIX_1_175875602
-	.word FIX_1_501321110
-	.word FIX_2_053119869
-	.word FIX_3_072711026
-	.word FIX_M_0_390180644
-	.word FIX_M_0_899976223
-	.word FIX_M_1_847759065
-	.word FIX_M_1_961570560
-	.word FIX_M_2_562915447
-	.word FIX_0xFFFF
+	END
Index: libavcodec/armv4l/simple_idct_arm.S
===================================================================
RCS file: /cvsroot/ffmpeg/ffmpeg/libavcodec/armv4l/simple_idct_arm.S,v
retrieving revision 1.1
diff -u -r1.1 simple_idct_arm.S
--- libavcodec/armv4l/simple_idct_arm.S	7 Jul 2003 11:19:18 -0000	1.1
+++ libavcodec/armv4l/simple_idct_arm.S	21 Jul 2005 04:57:30 -0000
@@ -1,485 +1,601 @@
-/*
- * simple_idct_arm.S
- * Copyright (C) 2002 Frederic 'dilb' Boulay.
- * All Rights Reserved.
- *
- * Author: Frederic Boulay <dilb@handhelds.org>
- *
- * You can redistribute this file and/or modify
- * it under the terms of the GNU General Public License (version 2)
- * as published by the Free Software Foundation.
- *
- * This file is distributed in the hope that it will be useful,
- * but WITHOUT ANY WARRANTY; without even the implied warranty of
- * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
- * GNU General Public License for more details.
- *
- * You should have received a copy of the GNU General Public License
- * along with this library; if not, write to the Free Software
- * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
- *
- *
- * The function defined in this file, is derived from the simple_idct function
- * from the libavcodec library part of the ffmpeg project.
- */
-
-/* useful constants for the algorithm, they are save in __constant_ptr__ at */
-/* the end of the source code.*/
-#define W1  22725
-#define W2  21407
-#define W3  19266
-#define W4  16383
-#define W5  12873
-#define W6  8867
-#define W7  4520
-#define MASK_MSHW 0xFFFF0000
-
-/* offsets of the constants in the vector */
-#define offW1  0
-#define offW2  4
-#define offW3  8
-#define offW4  12
-#define offW5  16
-#define offW6  20
-#define offW7  24
-#define offMASK_MSHW 28
-
-#define ROW_SHIFT 11
-#define ROW_SHIFT2MSHW (16-11)
-#define COL_SHIFT 20
-#define ROW_SHIFTED_1 1024 /* 1<< (ROW_SHIFT-1) */
-#define COL_SHIFTED_1 524288 /* 1<< (COL_SHIFT-1) */
-
-
-	.text
-	.align
-	.global simple_idct_ARM
-
-simple_idct_ARM:
-        @@ void simple_idct_ARM(int16_t *block)
-        @@ save stack for reg needed (take all of them),
-        @@ R0-R3 are scratch regs, so no need to save them, but R0 contains the pointer to block
-        @@ so it must not be overwritten, if it is not saved!!
-        @@ R12 is another scratch register, so it should not be saved too
-        @@ save all registers
-        stmfd sp!, {r4-r11, r14} @ R14 is also called LR
-        @@ at this point, R0=block, other registers are free.
-        add r14, r0, #112        @ R14=&block[8*7], better start from the last row, and decrease the value until row=0, i.e. R12=block.
-        add r12, pc, #(__constant_ptr__-.-8) @ R12=__constant_ptr__, the vector containing the constants, probably not necessary to reserve a register for it
-        @@ add 2 temporary variables in the stack: R0 and R14
-        sub sp, sp, #8          @ allow 2 local variables
-        str r0, [sp, #0]        @ save block in sp[0]
-        @@ stack status
-        @@ sp+4   free
-        @@ sp+0   R0  (block)
-
-
-        @@ at this point, R0=block, R14=&block[56], R12=__const_ptr_, R1-R11 free
-
-
-__row_loop:
-        @@ read the row and check if it is null, almost null, or not, according to strongarm specs, it is not necessary to optimise ldr accesses (i.e. split 32bits in 2 16bits words), at least it gives more usable registers :)
-        ldr r1, [r14, #0]        @ R1=(int32)(R12)[0]=ROWr32[0] (relative row cast to a 32b pointer)
-        ldr r2, [r14, #4]        @ R2=(int32)(R12)[1]=ROWr32[1]
-        ldr r3, [r14, #8]        @ R3=ROWr32[2]
-        ldr r4, [r14, #12]       @ R4=ROWr32[3]
-        @@ check if the words are null, if all of them are null, then proceed with next row (branch __end_row_loop),
-        @@ if ROWr16[0] is the only one not null, then proceed with this special case (branch __almost_empty_row)
-        @@ else follow the complete algorithm.
-        @@ at this point, R0=block, R14=&block[n], R12=__const_ptr_, R1=ROWr32[0], R2=ROWr32[1],
-        @@                R3=ROWr32[2], R4=ROWr32[3], R5-R11 free
-        orr r5, r4, r3           @ R5=R4 | R3
-        orr r5, r5, r2           @ R5=R4 | R3 | R2
-        orrs r6, r5, r1          @ Test R5 | R1 (the aim is to check if everything is null)
+;/*
+; * simple_idct_arm.S
+; * Copyright (C) 2002 Frederic 'dilb' Boulay.
+; * All Rights Reserved.
+; *
+; * Author: Frederic Boulay <dilb@handhelds.org>
+; *
+; * You can redistribute this file and/or modify
+; * it under the terms of the GNU General Public License (version 2)
+; * as published by the Free Software Foundation.
+; *
+; * This file is distributed in the hope that it will be useful,
+; * but WITHOUT ANY WARRANTY; without even the implied warranty of
+; * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
+; * GNU General Public License for more details.
+; *
+; * You should have received a copy of the GNU General Public License
+; * along with this library; if not, write to the Free Software
+; * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
+; *
+; *
+; * The function defined in this file, is derived from the simple_idct function
+; * from the libavcodec library part of the ffmpeg project.
+; */
+
+;/* useful constants for the algorithm, they are save in __constant_ptr__ at */
+;/* the end of the source code.*/
+W1  *  22725
+W2  *  21407
+W3  *  19266
+W4  *  16383
+W5  *  12873
+W6  *  8867
+W7  *  4520
+MASK_MSHW * 0xFFFF0000
+
+;/* offsets of the constants in the vector */
+offW1  *  0
+offW2  *  4
+offW3  *  8
+offW4  *  12
+offW5  *  16
+offW6  *  20
+offW7  *  24
+offMASK_MSHW * 28
+
+ROW_SHIFT * 11
+ROW_SHIFT2MSHW * (16-11)
+COL_SHIFT * 20
+ROW_SHIFTED_1 * 1024 ;/* 1<< (ROW_SHIFT-1) */
+COL_SHIFTED_1 * 524288 ;/* 1<< (COL_SHIFT-1) */
+
+	GBLL NO_HALFWORD
+NO_HALFWORD	SETL {TRUE}
+
+	AREA |C$$Code|, CODE, READONLY
+	ALIGN
+	EXPORT simple_idct_ARM
+
+simple_idct_ARM ROUT
+        ;; void simple_idct_ARM(int16_t *block)
+        ;; save stack for reg needed (take all of them),
+        ;; R0-R3 are scratch regs, so no need to save them, but R0 contains the pointer to block
+        ;; so it must not be overwritten, if it is not saved!!
+        ;; R12 is another scratch register, so it should not be saved too
+        ;; save all registers
+        stmfd sp!, {r4-r11, r14} ; R14 is also called LR
+        ;; at this point, R0=block, other registers are free.
+        add r14, r0, #112        ; R14=&block[8*7], better start from the last row, and decrease the value until row=0, i.e. R12=block.
+        add r12, pc, #(__constant_ptr__-.-8) ; R12=__constant_ptr__, the vector containing the constants, probably not necessary to reserve a register for it
+        ;; add 2 temporary variables in the stack: R0 and R14
+        sub sp, sp, #8          ; allow 2 local variables
+        str r0, [sp, #0]        ; save block in sp[0]
+        ;; stack status
+        ;; sp+4   free
+        ;; sp+0   R0  (block)
+
+
+        ;; at this point, R0=block, R14=&block[56], R12=__const_ptr_, R1-R11 free
+
+
+__row_loop
+        ;; read the row and check if it is null, almost null, or not, according to strongarm specs, it is not necessary to optimise ldr accesses (i.e. split 32bits in 2 16bits words), at least it gives more usable registers :)
+        ldr r1, [r14, #0]        ; R1=(int32)(R12)[0]=ROWr32[0] (relative row cast to a 32b pointer)
+        ldr r2, [r14, #4]        ; R2=(int32)(R12)[1]=ROWr32[1]
+        ldr r3, [r14, #8]        ; R3=ROWr32[2]
+        ldr r4, [r14, #12]       ; R4=ROWr32[3]
+        ;; check if the words are null, if all of them are null, then proceed with next row (branch __end_row_loop),
+        ;; if ROWr16[0] is the only one not null, then proceed with this special case (branch __almost_empty_row)
+        ;; else follow the complete algorithm.
+        ;; at this point, R0=block, R14=&block[n], R12=__const_ptr_, R1=ROWr32[0], R2=ROWr32[1],
+        ;;                R3=ROWr32[2], R4=ROWr32[3], R5-R11 free
+        orr r5, r4, r3           ; R5=R4 | R3
+        orr r5, r5, r2           ; R5=R4 | R3 | R2
+        orrs r6, r5, r1          ; Test R5 | R1 (the aim is to check if everything is null)
         beq __end_row_loop
-        mov r7, r1, asr #16      @ R7=R1>>16=ROWr16[1] (evaluate it now, as it could be useful later)
-        ldrsh r6, [r14, #0]      @ R6=ROWr16[0]
-        orrs r5, r5, r7          @ R5=R4 | R3 | R2 | R7
+        mov r7, r1, asr #16      ; R7=R1>>16=ROWr16[1] (evaluate it now, as it could be useful later)
+ [ NO_HALFWORD
+	ldr r6, [r14, #0]
+	mov r6, r6, LSL #16
+	mov r6, r6, ASR #16
+ |
+        ldrsh r6, [r14, #0]      ; R6=ROWr16[0]
+ ]
+        orrs r5, r5, r7          ; R5=R4 | R3 | R2 | R7
         beq __almost_empty_row

-__b_evaluation:
-        @@ at this point, R0=block (temp),  R1(free), R2=ROWr32[1], R3=ROWr32[2], R4=ROWr32[3],
-        @@     R5=(temp), R6=ROWr16[0], R7=ROWr16[1], R8-R11 free,
-        @@     R12=__const_ptr_, R14=&block[n]
-        @@ to save some registers/calls, proceed with b0-b3 first, followed by a0-a3
-
-        @@ MUL16(b0, W1, row[1]);
-        @@ MUL16(b1, W3, row[1]);
-        @@ MUL16(b2, W5, row[1]);
-        @@ MUL16(b3, W7, row[1]);
-        @@ MAC16(b0, W3, row[3]);
-        @@ MAC16(b1, -W7, row[3]);
-        @@ MAC16(b2, -W1, row[3]);
-        @@ MAC16(b3, -W5, row[3]);
-        ldr r8, [r12, #offW1]    @ R8=W1
-        mov r2, r2, asr #16      @ R2=ROWr16[3]
-        mul r0, r8, r7           @ R0=W1*ROWr16[1]=b0 (ROWr16[1] must be the second arg, to have the possibility to save 1 cycle)
-        ldr r9, [r12, #offW3]    @ R9=W3
-        ldr r10, [r12, #offW5]   @ R10=W5
-        mul r1, r9, r7           @ R1=W3*ROWr16[1]=b1 (ROWr16[1] must be the second arg, to have the possibility to save 1 cycle)
-        ldr r11, [r12, #offW7]   @ R11=W7
-        mul r5, r10, r7          @ R5=W5*ROWr16[1]=b2 (ROWr16[1] must be the second arg, to have the possibility to save 1 cycle)
-        mul r7, r11, r7          @ R7=W7*ROWr16[1]=b3 (ROWr16[1] must be the second arg, to have the possibility to save 1 cycle)
-		teq r2, #0               @ if null avoid muls
-		mlane r0, r9, r2, r0     @ R0+=W3*ROWr16[3]=b0 (ROWr16[3] must be the second arg, to have the possibility to save 1 cycle)
-        rsbne r2, r2, #0         @ R2=-ROWr16[3]
-        mlane r1, r11, r2, r1    @ R1-=W7*ROWr16[3]=b1 (ROWr16[3] must be the second arg, to have the possibility to save 1 cycle)
-        mlane r5, r8, r2, r5     @ R5-=W1*ROWr16[3]=b2 (ROWr16[3] must be the second arg, to have the possibility to save 1 cycle)
-        mlane r7, r10, r2, r7    @ R7-=W5*ROWr16[3]=b3 (ROWr16[3] must be the second arg, to have the possibility to save 1 cycle)
-
-        @@ at this point, R0=b0,  R1=b1, R2 (free), R3=ROWr32[2], R4=ROWr32[3],
-        @@     R5=b2, R6=ROWr16[0], R7=b3, R8=W1, R9=W3, R10=W5, R11=W7,
-        @@     R12=__const_ptr_, R14=&block[n]
-        @@ temp = ((uint32_t*)row)[2] | ((uint32_t*)row)[3];
-        @@ if (temp != 0) {}
-        orrs r2, r3, r4          @ R2=ROWr32[2] | ROWr32[3]
+__b_evaluation
+        ;; at this point, R0=block (temp),  R1(free), R2=ROWr32[1], R3=ROWr32[2], R4=ROWr32[3],
+        ;;     R5=(temp), R6=ROWr16[0], R7=ROWr16[1], R8-R11 free,
+        ;;     R12=__const_ptr_, R14=&block[n]
+        ;; to save some registers/calls, proceed with b0-b3 first, followed by a0-a3
+
+        ;; MUL16(b0, W1, row[1]);
+        ;; MUL16(b1, W3, row[1]);
+        ;; MUL16(b2, W5, row[1]);
+        ;; MUL16(b3, W7, row[1]);
+        ;; MAC16(b0, W3, row[3]);
+        ;; MAC16(b1, -W7, row[3]);
+        ;; MAC16(b2, -W1, row[3]);
+        ;; MAC16(b3, -W5, row[3]);
+        ldr r8, [r12, #offW1]    ; R8=W1
+        mov r2, r2, asr #16      ; R2=ROWr16[3]
+        mul r0, r8, r7           ; R0=W1*ROWr16[1]=b0 (ROWr16[1] must be the second arg, to have the possibility to save 1 cycle)
+        ldr r9, [r12, #offW3]    ; R9=W3
+        ldr r10, [r12, #offW5]   ; R10=W5
+        mul r1, r9, r7           ; R1=W3*ROWr16[1]=b1 (ROWr16[1] must be the second arg, to have the possibility to save 1 cycle)
+        ldr r11, [r12, #offW7]   ; R11=W7
+        mul r5, r10, r7          ; R5=W5*ROWr16[1]=b2 (ROWr16[1] must be the second arg, to have the possibility to save 1 cycle)
+        mul r7, r11, r7          ; R7=W7*ROWr16[1]=b3 (ROWr16[1] must be the second arg, to have the possibility to save 1 cycle)
+	teq r2, #0               ; if null avoid muls
+	mlane r0, r9, r2, r0     ; R0+=W3*ROWr16[3]=b0 (ROWr16[3] must be the second arg, to have the possibility to save 1 cycle)
+        rsbne r2, r2, #0         ; R2=-ROWr16[3]
+        mlane r1, r11, r2, r1    ; R1-=W7*ROWr16[3]=b1 (ROWr16[3] must be the second arg, to have the possibility to save 1 cycle)
+        mlane r5, r8, r2, r5     ; R5-=W1*ROWr16[3]=b2 (ROWr16[3] must be the second arg, to have the possibility to save 1 cycle)
+        mlane r7, r10, r2, r7    ; R7-=W5*ROWr16[3]=b3 (ROWr16[3] must be the second arg, to have the possibility to save 1 cycle)
+
+        ;; at this point, R0=b0,  R1=b1, R2 (free), R3=ROWr32[2], R4=ROWr32[3],
+        ;;     R5=b2, R6=ROWr16[0], R7=b3, R8=W1, R9=W3, R10=W5, R11=W7,
+        ;;     R12=__const_ptr_, R14=&block[n]
+        ;; temp = ((uint32_t*)row)[2] | ((uint32_t*)row)[3];
+        ;; if (temp != 0) {}
+        orrs r2, r3, r4          ; R2=ROWr32[2] | ROWr32[3]
         beq __end_b_evaluation

-        @@ at this point, R0=b0,  R1=b1, R2 (free), R3=ROWr32[2], R4=ROWr32[3],
-        @@     R5=b2, R6=ROWr16[0], R7=b3, R8=W1, R9=W3, R10=W5, R11=W7,
-        @@     R12=__const_ptr_, R14=&block[n]
-        @@ MAC16(b0, W5, row[5]);
-        @@ MAC16(b2, W7, row[5]);
-        @@ MAC16(b3, W3, row[5]);
-        @@ MAC16(b1, -W1, row[5]);
-        @@ MAC16(b0, W7, row[7]);
-        @@ MAC16(b2, W3, row[7]);
-        @@ MAC16(b3, -W1, row[7]);
-        @@ MAC16(b1, -W5, row[7]);
-        mov r3, r3, asr #16      @ R3=ROWr16[5]
-		teq r3, #0               @ if null avoid muls
-        mlane r0, r10, r3, r0    @ R0+=W5*ROWr16[5]=b0
-        mov r4, r4, asr #16      @ R4=ROWr16[7]
-        mlane r5, r11, r3, r5    @ R5+=W7*ROWr16[5]=b2
-        mlane r7, r9, r3, r7     @ R7+=W3*ROWr16[5]=b3
-        rsbne r3, r3, #0         @ R3=-ROWr16[5]
-        mlane r1, r8, r3, r1     @ R7-=W1*ROWr16[5]=b1
-        @@ R3 is free now
-		teq r4, #0               @ if null avoid muls
-        mlane r0, r11, r4, r0    @ R0+=W7*ROWr16[7]=b0
-        mlane r5, r9, r4, r5     @ R5+=W3*ROWr16[7]=b2
-        rsbne r4, r4, #0         @ R4=-ROWr16[7]
-        mlane r7, r8, r4, r7     @ R7-=W1*ROWr16[7]=b3
-        mlane r1, r10, r4, r1    @ R1-=W5*ROWr16[7]=b1
-        @@ R4 is free now
-__end_b_evaluation:
-        @@ at this point, R0=b0,  R1=b1, R2=ROWr32[2] | ROWr32[3] (tmp), R3 (free), R4 (free),
-        @@     R5=b2, R6=ROWr16[0], R7=b3, R8 (free), R9 (free), R10 (free), R11 (free),
-        @@     R12=__const_ptr_, R14=&block[n]
-
-__a_evaluation:
-        @@ a0 = (W4 * row[0]) + (1 << (ROW_SHIFT - 1));
-        @@ a1 = a0 + W6 * row[2];
-        @@ a2 = a0 - W6 * row[2];
-        @@ a3 = a0 - W2 * row[2];
-        @@ a0 = a0 + W2 * row[2];
-        ldr r9, [r12, #offW4]    @ R9=W4
-        mul r6, r9, r6           @ R6=W4*ROWr16[0]
-        ldr r10, [r12, #offW6]   @ R10=W6
-        ldrsh r4, [r14, #4]      @ R4=ROWr16[2] (a3 not defined yet)
-        add r6, r6, #ROW_SHIFTED_1 @ R6=W4*ROWr16[0] + 1<<(ROW_SHIFT-1) (a0)
-
-        mul r11, r10, r4         @ R11=W6*ROWr16[2]
-        ldr r8, [r12, #offW2]    @ R8=W2
-        sub r3, r6, r11          @ R3=a0-W6*ROWr16[2] (a2)
-        @@ temp = ((uint32_t*)row)[2] | ((uint32_t*)row)[3];
-        @@ if (temp != 0) {}
+        ;; at this point, R0=b0,  R1=b1, R2 (free), R3=ROWr32[2], R4=ROWr32[3],
+        ;;     R5=b2, R6=ROWr16[0], R7=b3, R8=W1, R9=W3, R10=W5, R11=W7,
+        ;;     R12=__const_ptr_, R14=&block[n]
+        ;; MAC16(b0, W5, row[5]);
+        ;; MAC16(b2, W7, row[5]);
+        ;; MAC16(b3, W3, row[5]);
+        ;; MAC16(b1, -W1, row[5]);
+        ;; MAC16(b0, W7, row[7]);
+        ;; MAC16(b2, W3, row[7]);
+        ;; MAC16(b3, -W1, row[7]);
+        ;; MAC16(b1, -W5, row[7]);
+        mov r3, r3, asr #16      ; R3=ROWr16[5]
+	teq r3, #0               ; if null avoid muls
+        mlane r0, r10, r3, r0    ; R0+=W5*ROWr16[5]=b0
+        mov r4, r4, asr #16      ; R4=ROWr16[7]
+        mlane r5, r11, r3, r5    ; R5+=W7*ROWr16[5]=b2
+        mlane r7, r9, r3, r7     ; R7+=W3*ROWr16[5]=b3
+        rsbne r3, r3, #0         ; R3=-ROWr16[5]
+        mlane r1, r8, r3, r1     ; R7-=W1*ROWr16[5]=b1
+        ;; R3 is free now
+	teq r4, #0               ; if null avoid muls
+        mlane r0, r11, r4, r0    ; R0+=W7*ROWr16[7]=b0
+        mlane r5, r9, r4, r5     ; R5+=W3*ROWr16[7]=b2
+        rsbne r4, r4, #0         ; R4=-ROWr16[7]
+        mlane r7, r8, r4, r7     ; R7-=W1*ROWr16[7]=b3
+        mlane r1, r10, r4, r1    ; R1-=W5*ROWr16[7]=b1
+        ;; R4 is free now
+__end_b_evaluation
+        ;; at this point, R0=b0,  R1=b1, R2=ROWr32[2] | ROWr32[3] (tmp), R3 (free), R4 (free),
+        ;;     R5=b2, R6=ROWr16[0], R7=b3, R8 (free), R9 (free), R10 (free), R11 (free),
+        ;;     R12=__const_ptr_, R14=&block[n]
+
+__a_evaluation
+        ;; a0 = (W4 * row[0]) + (1 << (ROW_SHIFT - 1));
+        ;; a1 = a0 + W6 * row[2];
+        ;; a2 = a0 - W6 * row[2];
+        ;; a3 = a0 - W2 * row[2];
+        ;; a0 = a0 + W2 * row[2];
+        ldr r9, [r12, #offW4]    ; R9=W4
+        mul r6, r9, r6           ; R6=W4*ROWr16[0]
+        ldr r10, [r12, #offW6]   ; R10=W6
+ [ NO_HALFWORD
+	ldr r4, [r14, #4]
+	mov r4, r4, LSL #16
+	mov r4, r4, ASR #16
+ |
+        ldrsh r4, [r14, #4]      ; R4=ROWr16[2] (a3 not defined yet)
+ ]
+        add r6, r6, #ROW_SHIFTED_1 ; R6=W4*ROWr16[0] + 1<<(ROW_SHIFT-1) (a0)
+
+        mul r11, r10, r4         ; R11=W6*ROWr16[2]
+        ldr r8, [r12, #offW2]    ; R8=W2
+        sub r3, r6, r11          ; R3=a0-W6*ROWr16[2] (a2)
+        ;; temp = ((uint32_t*)row)[2] | ((uint32_t*)row)[3];
+        ;; if (temp != 0) {}
         teq r2, #0
         beq __end_bef_a_evaluation

-	add r2, r6, r11          @ R2=a0+W6*ROWr16[2] (a1)
-        mul r11, r8, r4          @ R11=W2*ROWr16[2]
-        sub r4, r6, r11          @ R4=a0-W2*ROWr16[2] (a3)
-        add r6, r6, r11          @ R6=a0+W2*ROWr16[2] (a0)
-
-
-        @@ at this point, R0=b0,  R1=b1, R2=a1, R3=a2, R4=a3,
-        @@     R5=b2, R6=a0, R7=b3, R8=W2, R9=W4, R10=W6, R11 (free),
-        @@     R12=__const_ptr_, R14=&block[n]
-
-
-        @@ a0 += W4*row[4]
-        @@ a1 -= W4*row[4]
-        @@ a2 -= W4*row[4]
-        @@ a3 += W4*row[4]
-        ldrsh r11, [r14, #8]     @ R11=ROWr16[4]
-		teq r11, #0              @ if null avoid muls
-        mulne r11, r9, r11       @ R11=W4*ROWr16[4]
-        @@ R9 is free now
-        ldrsh r9, [r14, #12]     @ R9=ROWr16[6]
-        addne r6, r6, r11        @ R6+=W4*ROWr16[4] (a0)
-        subne r2, r2, r11        @ R2-=W4*ROWr16[4] (a1)
-        subne r3, r3, r11        @ R3-=W4*ROWr16[4] (a2)
-        addne r4, r4, r11        @ R4+=W4*ROWr16[4] (a3)
-        @@ W6 alone is no more useful, save W2*ROWr16[6] in it instead
-		teq r9, #0               @ if null avoid muls
-        mulne r11, r10, r9       @ R11=W6*ROWr16[6]
-        addne r6, r6, r11        @ R6+=W6*ROWr16[6] (a0)
-        mulne r10, r8, r9        @ R10=W2*ROWr16[6]
-        @@ a0 += W6*row[6];
-        @@ a3 -= W6*row[6];
-        @@ a1 -= W2*row[6];
-        @@ a2 += W2*row[6];
-        subne r4, r4, r11        @ R4-=W6*ROWr16[6] (a3)
-        subne r2, r2, r10        @ R2-=W2*ROWr16[6] (a1)
-        addne r3, r3, r10        @ R3+=W2*ROWr16[6] (a2)
-
-__end_a_evaluation:
-        @@ at this point, R0=b0,  R1=b1, R2=a1, R3=a2, R4=a3,
-        @@     R5=b2, R6=a0, R7=b3, R8 (free), R9 (free), R10 (free), R11 (free),
-        @@     R12=__const_ptr_, R14=&block[n]
-        @@ row[0] = (a0 + b0) >> ROW_SHIFT;
-        @@ row[1] = (a1 + b1) >> ROW_SHIFT;
-        @@ row[2] = (a2 + b2) >> ROW_SHIFT;
-        @@ row[3] = (a3 + b3) >> ROW_SHIFT;
-        @@ row[4] = (a3 - b3) >> ROW_SHIFT;
-        @@ row[5] = (a2 - b2) >> ROW_SHIFT;
-        @@ row[6] = (a1 - b1) >> ROW_SHIFT;
-        @@ row[7] = (a0 - b0) >> ROW_SHIFT;
-        add r8, r6, r0           @ R8=a0+b0
-        add r9, r2, r1           @ R9=a1+b1
-        @@ put 2 16 bits half-words in a 32bits word
-        @@ ROWr32[0]=ROWr16[0] | (ROWr16[1]<<16) (only Little Endian compliant then!!!)
-        ldr r10, [r12, #offMASK_MSHW] @ R10=0xFFFF0000
-        and r9, r10, r9, lsl #ROW_SHIFT2MSHW @ R9=0xFFFF0000 & ((a1+b1)<<5)
-        mvn r11, r10             @ R11= NOT R10= 0x0000FFFF
-        and r8, r11, r8, asr #ROW_SHIFT @ R8=0x0000FFFF & ((a0+b0)>>11)
+	add r2, r6, r11          ; R2=a0+W6*ROWr16[2] (a1)
+        mul r11, r8, r4          ; R11=W2*ROWr16[2]
+        sub r4, r6, r11          ; R4=a0-W2*ROWr16[2] (a3)
+        add r6, r6, r11          ; R6=a0+W2*ROWr16[2] (a0)
+
+
+        ;; at this point, R0=b0,  R1=b1, R2=a1, R3=a2, R4=a3,
+        ;;     R5=b2, R6=a0, R7=b3, R8=W2, R9=W4, R10=W6, R11 (free),
+        ;;     R12=__const_ptr_, R14=&block[n]
+
+
+        ;; a0 += W4*row[4]
+        ;; a1 -= W4*row[4]
+        ;; a2 -= W4*row[4]
+        ;; a3 += W4*row[4]
+ [ NO_HALFWORD
+	ldr r11, [r14, #8]
+	mov r11, r11, LSL #16
+	mov r11, r11, ASR #16
+ |
+        ldrsh r11, [r14, #8]     ; R11=ROWr16[4]
+ ]
+	teq r11, #0              ; if null avoid muls
+        mulne r11, r9, r11       ; R11=W4*ROWr16[4]
+        ;; R9 is free now
+ [ NO_HALFWORD
+	ldr r9, [r14, #12]
+	mov r9, r9, LSL #16
+	mov r9, r9, ASR #16
+ |
+        ldrsh r9, [r14, #12]     ; R9=ROWr16[6]
+ ]
+        addne r6, r6, r11        ; R6+=W4*ROWr16[4] (a0)
+        subne r2, r2, r11        ; R2-=W4*ROWr16[4] (a1)
+        subne r3, r3, r11        ; R3-=W4*ROWr16[4] (a2)
+        addne r4, r4, r11        ; R4+=W4*ROWr16[4] (a3)
+        ;; W6 alone is no more useful, save W2*ROWr16[6] in it instead
+		teq r9, #0               ; if null avoid muls
+        mulne r11, r10, r9       ; R11=W6*ROWr16[6]
+        addne r6, r6, r11        ; R6+=W6*ROWr16[6] (a0)
+        mulne r10, r8, r9        ; R10=W2*ROWr16[6]
+        ;; a0 += W6*row[6];
+        ;; a3 -= W6*row[6];
+        ;; a1 -= W2*row[6];
+        ;; a2 += W2*row[6];
+        subne r4, r4, r11        ; R4-=W6*ROWr16[6] (a3)
+        subne r2, r2, r10        ; R2-=W2*ROWr16[6] (a1)
+        addne r3, r3, r10        ; R3+=W2*ROWr16[6] (a2)
+
+__end_a_evaluation
+        ;; at this point, R0=b0,  R1=b1, R2=a1, R3=a2, R4=a3,
+        ;;     R5=b2, R6=a0, R7=b3, R8 (free), R9 (free), R10 (free), R11 (free),
+        ;;     R12=__const_ptr_, R14=&block[n]
+        ;; row[0] = (a0 + b0) >> ROW_SHIFT;
+        ;; row[1] = (a1 + b1) >> ROW_SHIFT;
+        ;; row[2] = (a2 + b2) >> ROW_SHIFT;
+        ;; row[3] = (a3 + b3) >> ROW_SHIFT;
+        ;; row[4] = (a3 - b3) >> ROW_SHIFT;
+        ;; row[5] = (a2 - b2) >> ROW_SHIFT;
+        ;; row[6] = (a1 - b1) >> ROW_SHIFT;
+        ;; row[7] = (a0 - b0) >> ROW_SHIFT;
+        add r8, r6, r0           ; R8=a0+b0
+        add r9, r2, r1           ; R9=a1+b1
+        ;; put 2 16 bits half-words in a 32bits word
+        ;; ROWr32[0]=ROWr16[0] | (ROWr16[1]<<16) (only Little Endian compliant then!!!)
+        ldr r10, [r12, #offMASK_MSHW] ; R10=0xFFFF0000
+        and r9, r10, r9, lsl #ROW_SHIFT2MSHW ; R9=0xFFFF0000 & ((a1+b1)<<5)
+        mvn r11, r10             ; R11= NOT R10= 0x0000FFFF
+        and r8, r11, r8, asr #ROW_SHIFT ; R8=0x0000FFFF & ((a0+b0)>>11)
         orr r8, r8, r9
         str r8, [r14, #0]

-        add r8, r3, r5           @ R8=a2+b2
-        add r9, r4, r7           @ R9=a3+b3
-        and r9, r10, r9, lsl #ROW_SHIFT2MSHW @ R9=0xFFFF0000 & ((a3+b3)<<5)
-        and r8, r11, r8, asr #ROW_SHIFT @ R8=0x0000FFFF & ((a2+b2)>>11)
+        add r8, r3, r5           ; R8=a2+b2
+        add r9, r4, r7           ; R9=a3+b3
+        and r9, r10, r9, lsl #ROW_SHIFT2MSHW ; R9=0xFFFF0000 & ((a3+b3)<<5)
+        and r8, r11, r8, asr #ROW_SHIFT ; R8=0x0000FFFF & ((a2+b2)>>11)
         orr r8, r8, r9
         str r8, [r14, #4]

-        sub r8, r4, r7           @ R8=a3-b3
-        sub r9, r3, r5           @ R9=a2-b2
-        and r9, r10, r9, lsl #ROW_SHIFT2MSHW @ R9=0xFFFF0000 & ((a2-b2)<<5)
-        and r8, r11, r8, asr #ROW_SHIFT @ R8=0x0000FFFF & ((a3-b3)>>11)
+        sub r8, r4, r7           ; R8=a3-b3
+        sub r9, r3, r5           ; R9=a2-b2
+        and r9, r10, r9, lsl #ROW_SHIFT2MSHW ; R9=0xFFFF0000 & ((a2-b2)<<5)
+        and r8, r11, r8, asr #ROW_SHIFT ; R8=0x0000FFFF & ((a3-b3)>>11)
         orr r8, r8, r9
         str r8, [r14, #8]

-        sub r8, r2, r1           @ R8=a1-b1
-        sub r9, r6, r0           @ R9=a0-b0
-        and r9, r10, r9, lsl #ROW_SHIFT2MSHW @ R9=0xFFFF0000 & ((a0-b0)<<5)
-        and r8, r11, r8, asr #ROW_SHIFT @ R8=0x0000FFFF & ((a1-b1)>>11)
+        sub r8, r2, r1           ; R8=a1-b1
+        sub r9, r6, r0           ; R9=a0-b0
+        and r9, r10, r9, lsl #ROW_SHIFT2MSHW ; R9=0xFFFF0000 & ((a0-b0)<<5)
+        and r8, r11, r8, asr #ROW_SHIFT ; R8=0x0000FFFF & ((a1-b1)>>11)
         orr r8, r8, r9
         str r8, [r14, #12]

         bal __end_row_loop

-__almost_empty_row:
-        @@ the row was empty, except ROWr16[0], now, management of this special case
-        @@ at this point, R0=block, R14=&block[n], R12=__const_ptr_, R1=ROWr32[0], R2=ROWr32[1],
-        @@                R3=ROWr32[2], R4=ROWr32[3], R5=(temp), R6=ROWr16[0], R7=ROWr16[1],
-        @@                R8=0xFFFF (temp), R9-R11 free
-        mov r8, #0x10000         @ R8=0xFFFF (2 steps needed!) it saves a ldr call (because of delay run).
-        sub r8, r8, #1           @ R8 is now ready.
-        and r5, r8, r6, lsl #3   @ R5=R8 & (R6<<3)= (ROWr16[0]<<3) & 0xFFFF
-        orr r5, r5, r5, lsl #16  @ R5=R5 | (R5<<16)
-        str r5, [r14, #0]        @ R14[0]=ROWr32[0]=R5
-        str r5, [r14, #4]        @ R14[4]=ROWr32[1]=R5
-        str r5, [r14, #8]        @ R14[8]=ROWr32[2]=R5
-        str r5, [r14, #12]       @ R14[12]=ROWr32[3]=R5
-
-__end_row_loop:
-        @@ at this point, R0-R11 (free)
-        @@     R12=__const_ptr_, R14=&block[n]
-        ldr r0, [sp, #0]         @ R0=block
-        teq r0, r14              @ compare current &block[8*n] to block, when block is reached, the loop is finished.
+__almost_empty_row
+        ;; the row was empty, except ROWr16[0], now, management of this special case
+        ;; at this point, R0=block, R14=&block[n], R12=__const_ptr_, R1=ROWr32[0], R2=ROWr32[1],
+        ;;                R3=ROWr32[2], R4=ROWr32[3], R5=(temp), R6=ROWr16[0], R7=ROWr16[1],
+        ;;                R8=0xFFFF (temp), R9-R11 free
+        mov r8, #0x10000         ; R8=0xFFFF (2 steps needed!) it saves a ldr call (because of delay run).
+        sub r8, r8, #1           ; R8 is now ready.
+        and r5, r8, r6, lsl #3   ; R5=R8 & (R6<<3)= (ROWr16[0]<<3) & 0xFFFF
+        orr r5, r5, r5, lsl #16  ; R5=R5 | (R5<<16)
+        str r5, [r14, #0]        ; R14[0]=ROWr32[0]=R5
+        str r5, [r14, #4]        ; R14[4]=ROWr32[1]=R5
+        str r5, [r14, #8]        ; R14[8]=ROWr32[2]=R5
+        str r5, [r14, #12]       ; R14[12]=ROWr32[3]=R5
+
+__end_row_loop
+        ;; at this point, R0-R11 (free)
+        ;;     R12=__const_ptr_, R14=&block[n]
+        ldr r0, [sp, #0]         ; R0=block
+        teq r0, r14              ; compare current &block[8*n] to block, when block is reached, the loop is finished.
         sub r14, r14, #16
         bne __row_loop



-	@@ at this point, R0=block, R1-R11 (free)
-	@@     R12=__const_ptr_, R14=&block[n]
-	add r14, r0, #14        @ R14=&block[7], better start from the last col, and decrease the value until col=0, i.e. R14=block.
-__col_loop:
-
-__b_evaluation2:
-	@@ at this point, R0=block (temp),  R1-R11 (free)
-	@@     R12=__const_ptr_, R14=&block[n]
-	@@ proceed with b0-b3 first, followed by a0-a3
-	@@ MUL16(b0, W1, col[8x1]);
-	@@ MUL16(b1, W3, col[8x1]);
-	@@ MUL16(b2, W5, col[8x1]);
-	@@ MUL16(b3, W7, col[8x1]);
-	@@ MAC16(b0, W3, col[8x3]);
-	@@ MAC16(b1, -W7, col[8x3]);
-	@@ MAC16(b2, -W1, col[8x3]);
-	@@ MAC16(b3, -W5, col[8x3]);
-	ldr r8, [r12, #offW1]    @ R8=W1
+	;; at this point, R0=block, R1-R11 (free)
+	;;     R12=__const_ptr_, R14=&block[n]
+	add r14, r0, #14        ; R14=&block[7], better start from the last col, and decrease the value until col=0, i.e. R14=block.
+__col_loop
+
+__b_evaluation2
+	;; at this point, R0=block (temp),  R1-R11 (free)
+	;;     R12=__const_ptr_, R14=&block[n]
+	;; proceed with b0-b3 first, followed by a0-a3
+	;; MUL16(b0, W1, col[8x1]);
+	;; MUL16(b1, W3, col[8x1]);
+	;; MUL16(b2, W5, col[8x1]);
+	;; MUL16(b3, W7, col[8x1]);
+	;; MAC16(b0, W3, col[8x3]);
+	;; MAC16(b1, -W7, col[8x3]);
+	;; MAC16(b2, -W1, col[8x3]);
+	;; MAC16(b3, -W5, col[8x3]);
+	ldr r8, [r12, #offW1]    ; R8=W1
+ [ NO_HALFWORD
+	ldr r7, [r14, #16]
+	mov r7, r7, LSL #16
+	mov r7, r7, ASR #16
+ |
 	ldrsh r7, [r14, #16]
-	mul r0, r8, r7           @ R0=W1*ROWr16[1]=b0 (ROWr16[1] must be the second arg, to have the possibility to save 1 cycle)
-	ldr r9, [r12, #offW3]    @ R9=W3
-	ldr r10, [r12, #offW5]   @ R10=W5
-	mul r1, r9, r7           @ R1=W3*ROWr16[1]=b1 (ROWr16[1] must be the second arg, to have the possibility to save 1 cycle)
-	ldr r11, [r12, #offW7]   @ R11=W7
-	mul r5, r10, r7          @ R5=W5*ROWr16[1]=b2 (ROWr16[1] must be the second arg, to have the possibility to save 1 cycle)
+ ]
+	mul r0, r8, r7           ; R0=W1*ROWr16[1]=b0 (ROWr16[1] must be the second arg, to have the possibility to save 1 cycle)
+	ldr r9, [r12, #offW3]    ; R9=W3
+	ldr r10, [r12, #offW5]   ; R10=W5
+	mul r1, r9, r7           ; R1=W3*ROWr16[1]=b1 (ROWr16[1] must be the second arg, to have the possibility to save 1 cycle)
+	ldr r11, [r12, #offW7]   ; R11=W7
+	mul r5, r10, r7          ; R5=W5*ROWr16[1]=b2 (ROWr16[1] must be the second arg, to have the possibility to save 1 cycle)
+ [ NO_HALFWORD
+	ldr r2, [r14, #48]
+	mov r2, r2, LSL #16
+	mov r2, r2, ASR #16
+ |
 	ldrsh r2, [r14, #48]
-	mul r7, r11, r7          @ R7=W7*ROWr16[1]=b3 (ROWr16[1] must be the second arg, to have the possibility to save 1 cycle)
-	teq r2, #0               @ if 0, then avoid muls
-	mlane r0, r9, r2, r0     @ R0+=W3*ROWr16[3]=b0 (ROWr16[3] must be the second arg, to have the possibility to save 1 cycle)
-	rsbne r2, r2, #0         @ R2=-ROWr16[3]
-	mlane r1, r11, r2, r1    @ R1-=W7*ROWr16[3]=b1 (ROWr16[3] must be the second arg, to have the possibility to save 1 cycle)
-	mlane r5, r8, r2, r5     @ R5-=W1*ROWr16[3]=b2 (ROWr16[3] must be the second arg, to have the possibility to save 1 cycle)
-	mlane r7, r10, r2, r7    @ R7-=W5*ROWr16[3]=b3 (ROWr16[3] must be the second arg, to have the possibility to save 1 cycle)
-
-	@@ at this point, R0=b0,  R1=b1, R2 (free), R3 (free), R4 (free),
-	@@     R5=b2, R6 (free), R7=b3, R8=W1, R9=W3, R10=W5, R11=W7,
-	@@     R12=__const_ptr_, R14=&block[n]
-	@@ MAC16(b0, W5, col[5x8]);
-	@@ MAC16(b2, W7, col[5x8]);
-	@@ MAC16(b3, W3, col[5x8]);
-	@@ MAC16(b1, -W1, col[5x8]);
-	@@ MAC16(b0, W7, col[7x8]);
-	@@ MAC16(b2, W3, col[7x8]);
-	@@ MAC16(b3, -W1, col[7x8]);
-	@@ MAC16(b1, -W5, col[7x8]);
-	ldrsh r3, [r14, #80]     @ R3=COLr16[5x8]
-	teq r3, #0               @ if 0 then avoid muls
-	mlane r0, r10, r3, r0    @ R0+=W5*ROWr16[5x8]=b0
-	mlane r5, r11, r3, r5    @ R5+=W7*ROWr16[5x8]=b2
-	mlane r7, r9, r3, r7     @ R7+=W3*ROWr16[5x8]=b3
-	rsbne r3, r3, #0         @ R3=-ROWr16[5x8]
-	ldrsh r4, [r14, #112]    @ R4=COLr16[7x8]
-	mlane r1, r8, r3, r1     @ R7-=W1*ROWr16[5x8]=b1
-	@@ R3 is free now
-	teq r4, #0               @ if 0 then avoid muls
-	mlane r0, r11, r4, r0    @ R0+=W7*ROWr16[7x8]=b0
-	mlane r5, r9, r4, r5     @ R5+=W3*ROWr16[7x8]=b2
-	rsbne r4, r4, #0         @ R4=-ROWr16[7x8]
-	mlane r7, r8, r4, r7     @ R7-=W1*ROWr16[7x8]=b3
-	mlane r1, r10, r4, r1    @ R1-=W5*ROWr16[7x8]=b1
-	@@ R4 is free now
-__end_b_evaluation2:
-	@@ at this point, R0=b0,  R1=b1, R2 (free), R3 (free), R4 (free),
-	@@     R5=b2, R6 (free), R7=b3, R8 (free), R9 (free), R10 (free), R11 (free),
-	@@     R12=__const_ptr_, R14=&block[n]
-
-__a_evaluation2:
-	@@ a0 = (W4 * col[8x0]) + (1 << (COL_SHIFT - 1));
-	@@ a1 = a0 + W6 * row[2];
-	@@ a2 = a0 - W6 * row[2];
-	@@ a3 = a0 - W2 * row[2];
-	@@ a0 = a0 + W2 * row[2];
+ ]
+	mul r7, r11, r7          ; R7=W7*ROWr16[1]=b3 (ROWr16[1] must be the second arg, to have the possibility to save 1 cycle)
+	teq r2, #0               ; if 0, then avoid muls
+	mlane r0, r9, r2, r0     ; R0+=W3*ROWr16[3]=b0 (ROWr16[3] must be the second arg, to have the possibility to save 1 cycle)
+	rsbne r2, r2, #0         ; R2=-ROWr16[3]
+	mlane r1, r11, r2, r1    ; R1-=W7*ROWr16[3]=b1 (ROWr16[3] must be the second arg, to have the possibility to save 1 cycle)
+	mlane r5, r8, r2, r5     ; R5-=W1*ROWr16[3]=b2 (ROWr16[3] must be the second arg, to have the possibility to save 1 cycle)
+	mlane r7, r10, r2, r7    ; R7-=W5*ROWr16[3]=b3 (ROWr16[3] must be the second arg, to have the possibility to save 1 cycle)
+
+	;; at this point, R0=b0,  R1=b1, R2 (free), R3 (free), R4 (free),
+	;;     R5=b2, R6 (free), R7=b3, R8=W1, R9=W3, R10=W5, R11=W7,
+	;;     R12=__const_ptr_, R14=&block[n]
+	;; MAC16(b0, W5, col[5x8]);
+	;; MAC16(b2, W7, col[5x8]);
+	;; MAC16(b3, W3, col[5x8]);
+	;; MAC16(b1, -W1, col[5x8]);
+	;; MAC16(b0, W7, col[7x8]);
+	;; MAC16(b2, W3, col[7x8]);
+	;; MAC16(b3, -W1, col[7x8]);
+	;; MAC16(b1, -W5, col[7x8]);
+ [ NO_HALFWORD
+	ldr r3, [r14, #80]
+	mov r3, r3, LSL #16
+	mov r3, r3, ASR #16
+ |
+	ldrsh r3, [r14, #80]     ; R3=COLr16[5x8]
+ ]
+	teq r3, #0               ; if 0 then avoid muls
+	mlane r0, r10, r3, r0    ; R0+=W5*ROWr16[5x8]=b0
+	mlane r5, r11, r3, r5    ; R5+=W7*ROWr16[5x8]=b2
+	mlane r7, r9, r3, r7     ; R7+=W3*ROWr16[5x8]=b3
+	rsbne r3, r3, #0         ; R3=-ROWr16[5x8]
+ [ NO_HALFWORD
+	ldr r4, [r14, #112]
+	mov r4, r4, LSL #16
+	mov r4, r4, ASR #16
+ |
+	ldrsh r4, [r14, #112]    ; R4=COLr16[7x8]
+ ]
+	mlane r1, r8, r3, r1     ; R7-=W1*ROWr16[5x8]=b1
+	;; R3 is free now
+	teq r4, #0               ; if 0 then avoid muls
+	mlane r0, r11, r4, r0    ; R0+=W7*ROWr16[7x8]=b0
+	mlane r5, r9, r4, r5     ; R5+=W3*ROWr16[7x8]=b2
+	rsbne r4, r4, #0         ; R4=-ROWr16[7x8]
+	mlane r7, r8, r4, r7     ; R7-=W1*ROWr16[7x8]=b3
+	mlane r1, r10, r4, r1    ; R1-=W5*ROWr16[7x8]=b1
+	;; R4 is free now
+__end_b_evaluation2
+	;; at this point, R0=b0,  R1=b1, R2 (free), R3 (free), R4 (free),
+	;;     R5=b2, R6 (free), R7=b3, R8 (free), R9 (free), R10 (free), R11 (free),
+	;;     R12=__const_ptr_, R14=&block[n]
+
+__a_evaluation2
+	;; a0 = (W4 * col[8x0]) + (1 << (COL_SHIFT - 1));
+	;; a1 = a0 + W6 * row[2];
+	;; a2 = a0 - W6 * row[2];
+	;; a3 = a0 - W2 * row[2];
+	;; a0 = a0 + W2 * row[2];
+ [ NO_HALFWORD
+	ldr r6, [r14, #0]
+	mov r6, r6, LSL #16
+	mov r6, r6, ASR #16
+ |
 	ldrsh r6, [r14, #0]
-	ldr r9, [r12, #offW4]    @ R9=W4
-	mul r6, r9, r6           @ R6=W4*ROWr16[0]
-	ldr r10, [r12, #offW6]   @ R10=W6
-	ldrsh r4, [r14, #32]      @ R4=ROWr16[2] (a3 not defined yet)
-	add r6, r6, #COL_SHIFTED_1 @ R6=W4*ROWr16[0] + 1<<(COL_SHIFT-1) (a0)
-	mul r11, r10, r4         @ R11=W6*ROWr16[2]
-	ldr r8, [r12, #offW2]    @ R8=W2
-	add r2, r6, r11          @ R2=a0+W6*ROWr16[2] (a1)
-	sub r3, r6, r11          @ R3=a0-W6*ROWr16[2] (a2)
-	mul r11, r8, r4          @ R11=W2*ROWr16[2]
-	sub r4, r6, r11          @ R4=a0-W2*ROWr16[2] (a3)
-	add r6, r6, r11          @ R6=a0+W2*ROWr16[2] (a0)
-
-	@@ at this point, R0=b0,  R1=b1, R2=a1, R3=a2, R4=a3,
-	@@     R5=b2, R6=a0, R7=b3, R8=W2, R9=W4, R10=W6, R11 (free),
-	@@     R12=__const_ptr_, R14=&block[n]
-	@@ a0 += W4*row[4]
-	@@ a1 -= W4*row[4]
-	@@ a2 -= W4*row[4]
-	@@ a3 += W4*row[4]
-	ldrsh r11, [r14, #64]     @ R11=ROWr16[4]
-	teq r11, #0              @ if null avoid muls
-	mulne r11, r9, r11       @ R11=W4*ROWr16[4]
-	@@ R9 is free now
-	addne r6, r6, r11        @ R6+=W4*ROWr16[4] (a0)
-	subne r2, r2, r11        @ R2-=W4*ROWr16[4] (a1)
-	subne r3, r3, r11        @ R3-=W4*ROWr16[4] (a2)
-	ldrsh r9, [r14, #96]     @ R9=ROWr16[6]
-	addne r4, r4, r11        @ R4+=W4*ROWr16[4] (a3)
-	@@ W6 alone is no more useful, save W2*ROWr16[6] in it instead
-	teq r9, #0               @ if null avoid muls
-	mulne r11, r10, r9       @ R11=W6*ROWr16[6]
-	addne r6, r6, r11        @ R6+=W6*ROWr16[6] (a0)
-	mulne r10, r8, r9        @ R10=W2*ROWr16[6]
-	@@ a0 += W6*row[6];
-	@@ a3 -= W6*row[6];
-	@@ a1 -= W2*row[6];
-	@@ a2 += W2*row[6];
-	subne r4, r4, r11        @ R4-=W6*ROWr16[6] (a3)
-	subne r2, r2, r10        @ R2-=W2*ROWr16[6] (a1)
-	addne r3, r3, r10        @ R3+=W2*ROWr16[6] (a2)
-__end_a_evaluation2:
-	@@ at this point, R0=b0,  R1=b1, R2=a1, R3=a2, R4=a3,
-	@@     R5=b2, R6=a0, R7=b3, R8 (free), R9 (free), R10 (free), R11 (free),
-	@@     R12=__const_ptr_, R14=&block[n]
-	@@ col[0 ] = ((a0 + b0) >> COL_SHIFT);
-	@@ col[8 ] = ((a1 + b1) >> COL_SHIFT);
-	@@ col[16] = ((a2 + b2) >> COL_SHIFT);
-	@@ col[24] = ((a3 + b3) >> COL_SHIFT);
-	@@ col[32] = ((a3 - b3) >> COL_SHIFT);
-	@@ col[40] = ((a2 - b2) >> COL_SHIFT);
-	@@ col[48] = ((a1 - b1) >> COL_SHIFT);
-	@@ col[56] = ((a0 - b0) >> COL_SHIFT);
-	@@@@@ no optimisation here @@@@@
-	add r8, r6, r0           @ R8=a0+b0
-	add r9, r2, r1           @ R9=a1+b1
+ ]
+	ldr r9, [r12, #offW4]    ; R9=W4
+	mul r6, r9, r6           ; R6=W4*ROWr16[0]
+	ldr r10, [r12, #offW6]   ; R10=W6
+ [ NO_HALFWORD
+	ldr r4, [r14, #32]
+	mov r4, r4, LSL #16
+	mov r4, r4, ASR #16
+ |
+	ldrsh r4, [r14, #32]      ; R4=ROWr16[2] (a3 not defined yet)
+ ]
+	add r6, r6, #COL_SHIFTED_1 ; R6=W4*ROWr16[0] + 1<<(COL_SHIFT-1) (a0)
+	mul r11, r10, r4         ; R11=W6*ROWr16[2]
+	ldr r8, [r12, #offW2]    ; R8=W2
+	add r2, r6, r11          ; R2=a0+W6*ROWr16[2] (a1)
+	sub r3, r6, r11          ; R3=a0-W6*ROWr16[2] (a2)
+	mul r11, r8, r4          ; R11=W2*ROWr16[2]
+	sub r4, r6, r11          ; R4=a0-W2*ROWr16[2] (a3)
+	add r6, r6, r11          ; R6=a0+W2*ROWr16[2] (a0)
+
+	;; at this point, R0=b0,  R1=b1, R2=a1, R3=a2, R4=a3,
+	;;     R5=b2, R6=a0, R7=b3, R8=W2, R9=W4, R10=W6, R11 (free),
+	;;     R12=__const_ptr_, R14=&block[n]
+	;; a0 += W4*row[4]
+	;; a1 -= W4*row[4]
+	;; a2 -= W4*row[4]
+	;; a3 += W4*row[4]
+ [ NO_HALFWORD
+	ldr r11, [r14, #64]
+	mov r11, r11, LSL #16
+	mov r11, r11, ASR #16
+ |
+	ldrsh r11, [r14, #64]     ; R11=ROWr16[4]
+ ]
+	teq r11, #0              ; if null avoid muls
+	mulne r11, r9, r11       ; R11=W4*ROWr16[4]
+	;; R9 is free now
+	addne r6, r6, r11        ; R6+=W4*ROWr16[4] (a0)
+	subne r2, r2, r11        ; R2-=W4*ROWr16[4] (a1)
+	subne r3, r3, r11        ; R3-=W4*ROWr16[4] (a2)
+ [ NO_HALFWORD
+	ldr r9, [r14, #96]
+	mov r9, r9, LSL #16
+	mov r9, r9, ASR #16
+ |
+	ldrsh r9, [r14, #96]     ; R9=ROWr16[6]
+ ]
+	addne r4, r4, r11        ; R4+=W4*ROWr16[4] (a3)
+	;; W6 alone is no more useful, save W2*ROWr16[6] in it instead
+	teq r9, #0               ; if null avoid muls
+	mulne r11, r10, r9       ; R11=W6*ROWr16[6]
+	addne r6, r6, r11        ; R6+=W6*ROWr16[6] (a0)
+	mulne r10, r8, r9        ; R10=W2*ROWr16[6]
+	;; a0 += W6*row[6];
+	;; a3 -= W6*row[6];
+	;; a1 -= W2*row[6];
+	;; a2 += W2*row[6];
+	subne r4, r4, r11        ; R4-=W6*ROWr16[6] (a3)
+	subne r2, r2, r10        ; R2-=W2*ROWr16[6] (a1)
+	addne r3, r3, r10        ; R3+=W2*ROWr16[6] (a2)
+__end_a_evaluation2
+	;; at this point, R0=b0,  R1=b1, R2=a1, R3=a2, R4=a3,
+	;;     R5=b2, R6=a0, R7=b3, R8 (free), R9 (free), R10 (free), R11 (free),
+	;;     R12=__const_ptr_, R14=&block[n]
+	;; col[0 ] = ((a0 + b0) >> COL_SHIFT);
+	;; col[8 ] = ((a1 + b1) >> COL_SHIFT);
+	;; col[16] = ((a2 + b2) >> COL_SHIFT);
+	;; col[24] = ((a3 + b3) >> COL_SHIFT);
+	;; col[32] = ((a3 - b3) >> COL_SHIFT);
+	;; col[40] = ((a2 - b2) >> COL_SHIFT);
+	;; col[48] = ((a1 - b1) >> COL_SHIFT);
+	;; col[56] = ((a0 - b0) >> COL_SHIFT);
+	;;;;; no optimisation here ;;;;;
+	add r8, r6, r0           ; R8=a0+b0
+	add r9, r2, r1           ; R9=a1+b1
 	mov r8, r8, asr #COL_SHIFT
 	mov r9, r9, asr #COL_SHIFT
+ [ NO_HALFWORD
+	strb r8, [r14, #0]
+	mov r8, r8, LSR #8
+	strb r8, [r14, #1]
+
+	strb r9, [r14, #16]
+	mov r9, r9, LSR #8
+	strb r9, [r14, #17]
+ |
 	strh r8, [r14, #0]
 	strh r9, [r14, #16]
-	add r8, r3, r5           @ R8=a2+b2
-	add r9, r4, r7           @ R9=a3+b3
+ ]
+	add r8, r3, r5           ; R8=a2+b2
+	add r9, r4, r7           ; R9=a3+b3
 	mov r8, r8, asr #COL_SHIFT
 	mov r9, r9, asr #COL_SHIFT
+ [ NO_HALFWORD
+	strb r8, [r14, #32]
+	mov r8, r8, LSR #8
+	strb r8, [r14, #33]
+
+	strb r9, [r14, #48]
+	mov r9, r9, LSR #8
+	strb r9, [r14, #49]
+ |
 	strh r8, [r14, #32]
 	strh r9, [r14, #48]
-	sub r8, r4, r7           @ R8=a3-b3
-	sub r9, r3, r5           @ R9=a2-b2
+ ]
+	sub r8, r4, r7           ; R8=a3-b3
+	sub r9, r3, r5           ; R9=a2-b2
 	mov r8, r8, asr #COL_SHIFT
 	mov r9, r9, asr #COL_SHIFT
+ [ NO_HALFWORD
+	strb r8, [r14, #64]
+	mov r8, r8, LSR #8
+	strb r8, [r14, #65]
+
+	strb r9, [r14, #80]
+	mov r9, r9, LSR #8
+	strb r9, [r14, #81]
+ |
 	strh r8, [r14, #64]
 	strh r9, [r14, #80]
-	sub r8, r2, r1           @ R8=a1-b1
-	sub r9, r6, r0           @ R9=a0-b0
+ ]
+	sub r8, r2, r1           ; R8=a1-b1
+	sub r9, r6, r0           ; R9=a0-b0
 	mov r8, r8, asr #COL_SHIFT
 	mov r9, r9, asr #COL_SHIFT
+ [ NO_HALFWORD
+	strb r8, [r14, #96]
+	mov r8, r8, LSR #8
+	strb r8, [r14, #97]
+
+	strb r9, [r14, #112]
+	mov r9, r9, LSR #8
+	strb r9, [r14, #113]
+ |
 	strh r8, [r14, #96]
 	strh r9, [r14, #112]
+ ]

-__end_col_loop:
-	@@ at this point, R0-R11 (free)
-	@@     R12=__const_ptr_, R14=&block[n]
-	ldr r0, [sp, #0]         @ R0=block
-	teq r0, r14              @ compare current &block[n] to block, when block is reached, the loop is finished.
+__end_col_loop
+	;; at this point, R0-R11 (free)
+	;;     R12=__const_ptr_, R14=&block[n]
+	ldr r0, [sp, #0]         ; R0=block
+	teq r0, r14              ; compare current &block[n] to block, when block is reached, the loop is finished.
 	sub r14, r14, #2
 	bne __col_loop




-__end_simple_idct_ARM:
-        @@ restore registers to previous status!
-        add sp, sp, #8 @@ the local variables!
-        ldmfd sp!, {r4-r11, r15} @@ update PC with LR content.
+__end_simple_idct_ARM
+        ;; restore registers to previous status!
+        add sp, sp, #8 ;; the local variables!
+        ldmfd sp!, {r4-r11, r15} ;; update PC with LR content.



-@@ kind of sub-function, here not to overload the common case.
-__end_bef_a_evaluation:
-	add r2, r6, r11          @ R2=a0+W6*ROWr16[2] (a1)
-        mul r11, r8, r4          @ R11=W2*ROWr16[2]
-        sub r4, r6, r11          @ R4=a0-W2*ROWr16[2] (a3)
-        add r6, r6, r11          @ R6=a0+W2*ROWr16[2] (a0)
+;; kind of sub-function, here not to overload the common case.
+__end_bef_a_evaluation
+	add r2, r6, r11          ; R2=a0+W6*ROWr16[2] (a1)
+        mul r11, r8, r4          ; R11=W2*ROWr16[2]
+        sub r4, r6, r11          ; R4=a0-W2*ROWr16[2] (a3)
+        add r6, r6, r11          ; R6=a0+W2*ROWr16[2] (a0)
 	bal __end_a_evaluation


-__constant_ptr__:  @@ see #defines at the beginning of the source code for values.
-	.align
-        .word   W1
-        .word   W2
-        .word   W3
-        .word   W4
-        .word   W5
-        .word   W6
-        .word   W7
-        .word   MASK_MSHW
+__constant_ptr__  ;; see #defines at the beginning of the source code for values.
+	ALIGN
+        DCD   W1
+        DCD   W2
+        DCD   W3
+        DCD   W4
+        DCD   W5
+        DCD   W6
+        DCD   W7
+        DCD   MASK_MSHW
+
+	END
