--- config/arm/common.h.old	2003-01-28 18:49:36.000000000 +0000
+++ config/arm/common.h	2003-01-28 18:48:54.000000000 +0000
@@ -79,7 +79,7 @@
         : "=r" (r0), "=r" (r1), "=f" (f0)				\
 	: "r" ((CALL)->function),					\
 	  "0" (r0), "1" (r1), "r" (r2), "r" (r3) 			\
-	: "ip", "rfp", "sl", "fp", "lr" 				\
+	: "ip", "fp", "sl", "fp", "lr" 				\
 	);								\
     switch((CALL)->rettype)    						\
     {									\
