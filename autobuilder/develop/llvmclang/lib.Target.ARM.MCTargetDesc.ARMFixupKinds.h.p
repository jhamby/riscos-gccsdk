--- lib/Target/ARM/MCTargetDesc/ARMFixupKinds.h.orig	2014-03-29 06:26:49.000000000 +0000
+++ lib/Target/ARM/MCTargetDesc/ARMFixupKinds.h	2014-11-01 19:09:13.724130903 +0000
@@ -100,6 +100,9 @@
   fixup_t2_movt_hi16, // :upper16:
   fixup_t2_movw_lo16, // :lower16:
 
+  fixup_riscos_gott_base,
+  fixup_riscos_gott_index,
+
   // Marker
   LastTargetFixupKind,
   NumTargetFixupKinds = LastTargetFixupKind - FirstTargetFixupKind
