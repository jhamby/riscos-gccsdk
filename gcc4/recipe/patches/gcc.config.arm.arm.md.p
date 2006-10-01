--- gcc/config/arm/arm.md.orig	2006-09-24 23:20:22.000000000 +0200
+++ gcc/config/arm/arm.md	2006-09-24 23:39:44.000000000 +0200
@@ -31,6 +31,7 @@
 ;; Register numbers
 (define_constants
   [(R0_REGNUM        0)		; First CORE register
+   (SL_REGNUM	    10)		; Stack limit register
    (IP_REGNUM	    12)		; Scratch register
    (SP_REGNUM	    13)		; Stack pointer
    (LR_REGNUM       14)		; Return address register
@@ -93,6 +94,10 @@
    (UNSPEC_TLS      20) ; A symbol that has been treated properly for TLS usage.
    (UNSPEC_PIC_LABEL 21) ; A label used for PIC access that does not appear in the
                          ; instruction stream.
+   (UNSPEC_STK 22)
+   (UNSPEC_STK_BIG 23)
+   (UNSPEC_SWI 24)
+   (UNSPEC_CALL 25)
   ]
 )
 
@@ -10173,4 +10178,5 @@
 (include "iwmmxt.md")
 ;; Load the VFP co-processor patterns
 (include "vfp.md")
-
+;; Load the RISC OS patterns
+(include "riscos.md")
