--- gcc/config/arm/arm.md.orig	2006-09-27 19:10:22.000000000 +0200
+++ gcc/config/arm/arm.md	2010-07-04 19:34:08.694402306 +0200
@@ -31,6 +31,7 @@
 ;; Register numbers
 (define_constants
   [(R0_REGNUM        0)		; First CORE register
+   (SL_REGNUM	    10)		; Stack limit register (APCS-32 only)
    (IP_REGNUM	    12)		; Scratch register
    (SP_REGNUM	    13)		; Stack pointer
    (LR_REGNUM       14)		; Return address register
@@ -93,6 +94,8 @@
    (UNSPEC_TLS      20) ; A symbol that has been treated properly for TLS usage.
    (UNSPEC_PIC_LABEL 21) ; A label used for PIC access that does not appear in the
                          ; instruction stream.
+   (UNSPEC_STK 22)
+   (UNSPEC_CALL 23)
   ]
 )
 
@@ -7586,7 +7589,7 @@
       return thumb_call_via_reg (operands[0]);
     else if (operands[1] == const0_rtx)
       return \"bl\\t%__interwork_call_via_%0\";
-    else if (frame_pointer_needed)
+    else if (arm_apcs_frame_needed ())
       return \"bl\\t%__interwork_r7_call_via_%0\";
     else
       return \"bl\\t%__interwork_r11_call_via_%0\";
@@ -7682,7 +7685,7 @@
       return thumb_call_via_reg (operands[1]);
     else if (operands[2] == const0_rtx)
       return \"bl\\t%__interwork_call_via_%1\";
-    else if (frame_pointer_needed)
+    else if (arm_apcs_frame_needed ())
       return \"bl\\t%__interwork_r7_call_via_%1\";
     else
       return \"bl\\t%__interwork_r11_call_via_%1\";
@@ -10184,4 +10187,5 @@
 (include "iwmmxt.md")
 ;; Load the VFP co-processor patterns
 (include "vfp.md")
-
+;; Load the RISC OS patterns
+(include "riscos.md")
