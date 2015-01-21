--- lib/Target/ARM/MCTargetDesc/ARMMCAsmInfo.cpp.orig	2014-05-07 08:49:34.000000000 +0100
+++ lib/Target/ARM/MCTargetDesc/ARMMCAsmInfo.cpp	2014-10-17 12:10:37.422247127 +0100
@@ -47,6 +47,9 @@
       (TheTriple.getArch() == Triple::thumbeb))
     IsLittleEndian = false;
 
+  if (TheTriple.getOS() == Triple::RISCOS)
+    HasBigEndianDoubles = true;
+
   // ".comm align is in bytes but .align is pow-2."
   AlignmentIsInBytes = false;
 
