--- lib/Target/ARM/MCTargetDesc/ARMELFObjectWriter.cpp.orig	2014-07-21 00:15:06.000000000 +0100
+++ lib/Target/ARM/MCTargetDesc/ARMELFObjectWriter.cpp	2014-11-10 20:21:26.742364030 +0000
@@ -167,6 +167,11 @@
         Type = ELF::R_ARM_TLS_IE32;
         break;
       case MCSymbolRefExpr::VK_None:
+        if (/*RISCOS && */Target.getSymA()->getSymbol().getName() == "__GOTT_BASE__") {
+          Type = ELF::R_ARM_NONE;
+          break;
+        }
+
         Type = ELF::R_ARM_ABS32;
         break;
       case MCSymbolRefExpr::VK_GOTOFF:
@@ -222,6 +227,10 @@
     case ARM::fixup_t2_movw_lo16:
       Type = ELF::R_ARM_THM_MOVW_ABS_NC;
       break;
+    case ARM::fixup_riscos_gott_index:
+    case ARM::fixup_riscos_gott_base:
+      Type = ELF::R_ARM_NONE;
+      break;
     }
   }
 
