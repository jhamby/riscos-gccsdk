--- gcc/config/arm/unspecs.md.orig	2018-05-09 17:13:19.672790611 +0100
+++ gcc/config/arm/unspecs.md	2018-05-09 19:04:08.800981296 +0100
@@ -86,6 +86,8 @@
   UNSPEC_PROBE_STACK    ; Probe stack memory reference
   UNSPEC_NONSECURE_MEM	; Represent non-secure memory in ARMv8-M with
 			; security extension
+  UNSPEC_STK            ; RISC OS port.
+  UNSPEC_CALL           ; RISC OS port.
 ])
 
 (define_c_enum "unspec" [
