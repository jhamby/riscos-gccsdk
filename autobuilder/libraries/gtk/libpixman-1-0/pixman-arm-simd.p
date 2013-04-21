--- pixman/pixman-arm-simd.c.orig	2012-05-14 18:40:58.000000000 +0100
+++ pixman/pixman-arm-simd.c	2013-04-21 16:11:02.000000000 +0100
@@ -143,18 +143,18 @@
 	    "sub r8, %[alpha_mask], r5, lsr #24\n\t"
 #endif
 	    "uxtb16 r6, r4\n\t"
-	    "uxtb16 r7, r4, ror #8\n\t"
+			"uxtb16 r9, r4, ror #8\n\t"
 
 	    /* multiply by 257 and divide by 65536 */
 	    "mla r6, r6, r8, %[component_half]\n\t"
-	    "mla r7, r7, r8, %[component_half]\n\t"
+			"mla r9, r9, r8, %[component_half]\n\t"
 
 	    "uxtab16 r6, r6, r6, ror #8\n\t"
-	    "uxtab16 r7, r7, r7, ror #8\n\t"
+			"uxtab16 r9, r9, r9, ror #8\n\t"
 
-	    /* recombine the 0xff00ff00 bytes of r6 and r7 */
-	    "and r7, r7, %[upper_component_mask]\n\t"
-	    "uxtab16 r6, r7, r6, ror #8\n\t"
+			/* recombine the 0xff00ff00 bytes of r6 and r9 */
+			"and r9, r9, %[upper_component_mask]\n\t"
+			"uxtab16 r6, r9, r6, ror #8\n\t"
 
 	    "uqadd8 r5, r6, r5\n\t"
 
@@ -170,7 +170,7 @@
 	    : [w] "+r" (w), [dest] "+r" (dst), [src] "+r" (src)
 	    : [component_half] "r" (component_half), [upper_component_mask] "r" (upper_component_mask),
 	      [alpha_mask] "r" (alpha_mask)
-	    : "r4", "r5", "r6", "r7", "r8", "cc", "memory"
+			: "r4", "r5", "r6", "r8", "r9", "cc", "memory"
 	    );
     }
 }
@@ -219,39 +219,39 @@
 	    "ldr r4, [%[dest]] \n\t"
 
 	    "uxtb16 r6, r5\n\t"
-	    "uxtb16 r7, r5, ror #8\n\t"
+			"uxtb16 r10, r5, ror #8\n\t"
 
 	    /* multiply by alpha (r8) then by 257 and divide by 65536 */
 	    "mla r6, r6, %[mask_alpha], %[component_half]\n\t"
-	    "mla r7, r7, %[mask_alpha], %[component_half]\n\t"
+			"mla r10, r10, %[mask_alpha], %[component_half]\n\t"
 
 	    "uxtab16 r6, r6, r6, ror #8\n\t"
-	    "uxtab16 r7, r7, r7, ror #8\n\t"
+			"uxtab16 r10, r10, r10, ror #8\n\t"
 
 	    "uxtb16 r6, r6, ror #8\n\t"
-	    "uxtb16 r7, r7, ror #8\n\t"
+			"uxtb16 r10, r10, ror #8\n\t"
 
 	    /* recombine */
-	    "orr r5, r6, r7, lsl #8\n\t"
+			"orr r5, r6, r10, lsl #8\n\t"
 
 	    "uxtb16 r6, r4\n\t"
-	    "uxtb16 r7, r4, ror #8\n\t"
+			"uxtb16 r10, r4, ror #8\n\t"
 
 	    /* 255 - alpha */
 	    "sub r8, %[alpha_mask], r5, lsr #24\n\t"
 
 	    /* multiply by alpha (r8) then by 257 and divide by 65536 */
 	    "mla r6, r6, r8, %[component_half]\n\t"
-	    "mla r7, r7, r8, %[component_half]\n\t"
+			"mla r10, r10, r8, %[component_half]\n\t"
 
 	    "uxtab16 r6, r6, r6, ror #8\n\t"
-	    "uxtab16 r7, r7, r7, ror #8\n\t"
+			"uxtab16 r10, r10, r10, ror #8\n\t"
 
 	    "uxtb16 r6, r6, ror #8\n\t"
-	    "uxtb16 r7, r7, ror #8\n\t"
+			"uxtb16 r10, r10, ror #8\n\t"
 
 	    /* recombine */
-	    "orr r6, r6, r7, lsl #8\n\t"
+			"orr r6, r6, r10, lsl #8\n\t"
 
 	    "uqadd8 r5, r6, r5\n\t"
 
@@ -267,7 +267,7 @@
 	    : [w] "+r" (w), [dest] "+r" (dst), [src] "+r" (src)
 	    : [component_half] "r" (component_half), [mask_alpha] "r" (mask),
 	      [alpha_mask] "r" (alpha_mask)
-	    : "r4", "r5", "r6", "r7", "r8", "r9", "cc", "memory"
+			: "r4", "r5", "r6", "r8", "r9", "r10", "cc", "memory"
 	    );
     }
 }
@@ -321,21 +321,24 @@
 #endif
 	    "ldr r4, [%[dest]] \n\t"
 
+			"mov r10, #0x800000\n\t"
+			"orr r10, r10, #0x80\n\t"
+
 	    /* multiply by alpha (r8) then by 257 and divide by 65536 */
-	    "mla r6, %[src_lo], r5, %[component_half]\n\t"
-	    "mla r7, %[src_hi], r5, %[component_half]\n\t"
+			"mla r6, %[src_lo], r5, r10\n\t"
+			"mla r9, %[src_hi], r5, r10\n\t"
 
 	    "uxtab16 r6, r6, r6, ror #8\n\t"
-	    "uxtab16 r7, r7, r7, ror #8\n\t"
+			"uxtab16 r9, r9, r9, ror #8\n\t"
 
 	    "uxtb16 r6, r6, ror #8\n\t"
-	    "uxtb16 r7, r7, ror #8\n\t"
+			"uxtb16 r9, r9, ror #8\n\t"
 
 	    /* recombine */
-	    "orr r5, r6, r7, lsl #8\n\t"
+			"orr r5, r6, r9, lsl #8\n\t"
 
 	    "uxtb16 r6, r4\n\t"
-	    "uxtb16 r7, r4, ror #8\n\t"
+			"uxtb16 r9, r4, ror #8\n\t"
 
 	    /* we could simplify this to use 'sub' if we were
 	     * willing to give up a register for alpha_mask
@@ -344,17 +347,17 @@
 	    "mov r8, r8, lsr #24\n\t"
 
 	    /* multiply by alpha (r8) then by 257 and divide by 65536 */
-	    "mla r6, r6, r8, %[component_half]\n\t"
-	    "mla r7, r7, r8, %[component_half]\n\t"
+			"mla r6, r6, r8, r10\n\t"
+			"mla r9, r9, r8, r10\n\t"
 
 	    "uxtab16 r6, r6, r6, ror #8\n\t"
-	    "uxtab16 r7, r7, r7, ror #8\n\t"
+			"uxtab16 r9, r9, r9, ror #8\n\t"
 
 	    "uxtb16 r6, r6, ror #8\n\t"
-	    "uxtb16 r7, r7, ror #8\n\t"
+			"uxtb16 r9, r9, ror #8\n\t"
 
 	    /* recombine */
-	    "orr r6, r6, r7, lsl #8\n\t"
+			"orr r6, r6, r9, lsl #8\n\t"
 
 	    "uqadd8 r5, r6, r5\n\t"
 
@@ -370,7 +373,7 @@
 	    : [w] "+r" (w), [dest] "+r" (dst), [src] "+r" (src), [mask] "+r" (mask)
 	    : [component_half] "r" (component_half),
 	      [src_hi] "r" (src_hi), [src_lo] "r" (src_lo)
-	    : "r4", "r5", "r6", "r7", "r8", "cc", "memory");
+	    : "r4", "r5", "r6", "r8", "r9", "r10", "cc", "memory");
     }
 }
 
