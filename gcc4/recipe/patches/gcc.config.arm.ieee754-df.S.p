--- gcc/config/arm/ieee754-df.S.orig	2006-12-13 02:54:07.000000000 +0100
+++ gcc/config/arm/ieee754-df.S	2006-12-13 02:53:40.000000000 +0100
@@ -528,7 +528,7 @@
 LSYM(f0_ret):
 	stmfd	sp!, {r0, r1}
 	ldfd	f0, [sp], #8
-	RETLDM
+	ldr	pc, [sp], #4
 
 #endif
 
