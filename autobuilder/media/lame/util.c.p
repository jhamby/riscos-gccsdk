--- libmp3lame/util.c.orig	2011-02-18 23:44:35.000000000 +0000
+++ libmp3lame/util.c	2011-02-18 23:44:01.000000000 +0000
@@ -830,7 +830,7 @@
     }
 #endif
 
-#if defined(__riscos__) && !defined(ABORTFP)
+/* #if defined(__riscos__) && !defined(ABORTFP) */
     /* Disable FPE's under RISC OS */
     /* if bit is set, we disable trapping that error! */
     /*   _FPE_IVO : invalid operation */
@@ -838,8 +838,8 @@
     /*   _FPE_OFL : overflow */
     /*   _FPE_UFL : underflow */
     /*   _FPE_INX : inexact */
-    DisableFPETraps(_FPE_IVO | _FPE_DVZ | _FPE_OFL);
-#endif
+/*    DisableFPETraps(_FPE_IVO | _FPE_DVZ | _FPE_OFL);
+#endif */
 
     /*
      *  Debugging stuff
