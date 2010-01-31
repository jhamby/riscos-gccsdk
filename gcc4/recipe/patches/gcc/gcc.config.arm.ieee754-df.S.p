--- gcc/config/arm/ieee754-df.S.orig	2006-12-14 00:39:19.000000000 +0100
+++ gcc/config/arm/ieee754-df.S	2006-12-13 22:24:25.000000000 +0100
@@ -460,7 +460,8 @@
 	@ we can return the result in f0 as well as in r0/r1 for backwards
 	@ compatibility.
 	adr	ip, LSYM(f0_ret)
-	stmfd	sp!, {r4, r5, ip, lr}
+	@ Push pc as well so that RETLDM works correctly.
+	stmfd	sp!, {r4, r5, ip, lr, pc}
 #else
 	stmfd	sp!, {r4, r5, lr}
 #endif
@@ -482,7 +483,8 @@
 	@ we can return the result in f0 as well as in r0/r1 for backwards
 	@ compatibility.
 	adr	ip, LSYM(f0_ret)
-	stmfd	sp!, {r4, r5, ip, lr}
+	@ Push pc as well so that RETLDM works correctly.
+	stmfd	sp!, {r4, r5, ip, lr, pc}
 #else
 	stmfd	sp!, {r4, r5, lr}
 #endif
