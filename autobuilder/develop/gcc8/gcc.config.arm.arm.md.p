--- gcc/config/arm/arm.md.orig	2018-03-23 16:43:43.000000000 +0000
+++ gcc/config/arm/arm.md	2018-05-09 19:43:11.825048489 +0100
@@ -31,6 +31,7 @@
   [(R0_REGNUM         0)	; First CORE register
    (R1_REGNUM	      1)	; Second CORE register
    (R4_REGNUM	      4)	; Fifth CORE register
+   (SL_REGNUM	     10)	; Stack limit register (APCS-32 only)
    (IP_REGNUM	     12)	; Scratch register
    (SP_REGNUM	     13)	; Stack pointer
    (LR_REGNUM        14)	; Return address register
@@ -6296,7 +6297,7 @@
 	(mem:SI (plus:SI (match_operand:SI 1 "register_operand" "r")
 			 (unspec:SI [(match_operand:SI 2 "" "X")]
 				    UNSPEC_PIC_OFFSET))))]
-  "TARGET_VXWORKS_RTP && TARGET_ARM && flag_pic"
+  "(TARGET_VXWORKS_RTP && TARGET_ARM && flag_pic) || flag_pic == 2"
   "ldr%?\\t%0, [%1,%2]"
   [(set_attr "type" "load_4")]
 )
