--- lib/MC/MCAsmInfo.cpp.orig	2014-06-24 23:45:16.000000000 +0100
+++ lib/MC/MCAsmInfo.cpp	2014-10-17 12:07:15.222241328 +0100
@@ -27,6 +27,7 @@
   CalleeSaveStackSlotSize = 4;
 
   IsLittleEndian = true;
+  HasBigEndianDoubles = false;
   StackGrowsUp = false;
   HasSubsectionsViaSymbols = false;
   HasMachoZeroFillDirective = false;
