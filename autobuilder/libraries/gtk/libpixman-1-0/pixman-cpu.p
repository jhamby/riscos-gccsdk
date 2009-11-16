--- pixman/pixman-cpu.c.orig	2009-11-16 13:06:35.000000000 -0800
+++ pixman/pixman-cpu.c	2009-11-16 13:07:54.000000000 -0800
@@ -221,7 +221,7 @@
 
 #endif /* USE_ARM_NEON */
 
-#else /* linux ELF */
+#elif !defined(__riscos__) /* linux ELF */
 
 #include <stdlib.h>
 #include <unistd.h>
