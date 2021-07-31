--- include/llvm/MC/MCAsmInfo.h.orig	2014-06-29 22:43:47.000000000 +0100
+++ include/llvm/MC/MCAsmInfo.h	2014-10-17 13:24:07.422373597 +0100
@@ -71,6 +71,9 @@
   /// True if target is little endian.  Default is true.
   bool IsLittleEndian;
 
+  /// True if target has big endian doubles (regardless of IsLittleEndian above).
+  bool HasBigEndianDoubles;
+
   /// True if target stack grow up.  Default is false.
   bool StackGrowsUp;
 
@@ -356,6 +359,9 @@
   /// True if the target is little endian.
   bool isLittleEndian() const { return IsLittleEndian; }
 
+  /// True if the target has big endian doubles (regardless of isLittleEndian above).
+  bool hasBigEndianDoubles() const { return HasBigEndianDoubles; }
+
   /// True if target stack grow up.
   bool isStackGrowthDirectionUp() const { return StackGrowsUp; }
 
