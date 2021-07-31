--- gcc/config/arm/unspecs.md.orig	2020-07-24 17:16:30.821914653 +0100
+++ gcc/config/arm/unspecs.md	2020-07-24 21:49:11.568234080 +0100
@@ -158,6 +158,8 @@
   UNSPEC_CDEA		; Custom Datapath Extension instruction.
   UNSPEC_VCDE		; Custom Datapath Extension instruction.
   UNSPEC_VCDEA		; Custom Datapath Extension instruction.
+  UNSPEC_STK            ; RISC OS port.
+  UNSPEC_CALL           ; RISC OS port.
 ])
 
 
