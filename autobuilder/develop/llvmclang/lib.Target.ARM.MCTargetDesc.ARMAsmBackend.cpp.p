--- lib/Target/ARM/MCTargetDesc/ARMAsmBackend.cpp.orig	2014-05-20 10:24:37.000000000 +0100
+++ lib/Target/ARM/MCTargetDesc/ARMAsmBackend.cpp	2014-11-10 19:22:16.402318023 +0000
@@ -586,6 +586,9 @@
 
     return Value;
   }
+  case ARM::fixup_riscos_gott_index: {
+    return Value;
+  }
   }
 }
 
@@ -674,6 +677,7 @@
   case ARM::fixup_arm_movw_lo16:
   case ARM::fixup_t2_movt_hi16:
   case ARM::fixup_t2_movw_lo16:
+  case ARM::fixup_riscos_gott_index:
     return 4;
 
   case FK_SecRel_2:
