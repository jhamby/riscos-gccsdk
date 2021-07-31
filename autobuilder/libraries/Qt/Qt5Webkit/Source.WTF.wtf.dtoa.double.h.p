--- Source/WTF/wtf/dtoa/double.h.orig	2015-06-29 21:11:01.000000000 +0100
+++ Source/WTF/wtf/dtoa/double.h	2016-04-03 11:51:40.124219300 +0100
@@ -34,9 +34,24 @@
 
 namespace double_conversion {
     
+#if OS(RISCOS) && !(defined(__VFP_FP__) && !defined(__SOFTFP__))
+    // A double has most significant word first, which is the opposite
+    // to a uint64_t? Have to swap the words over.
+    static uint64_t double_to_uint64(double d) {
+      uint64_t r = BitCast<uint64_t>(d);
+      uint32_t high = r >> 32;
+      return (r << 32) | high;
+    }
+    static double uint64_to_double(uint64_t d64) {
+      uint32_t high = d64 >> 32;
+      d64 = (d64 << 32) | high;
+      return BitCast<double>(d64);
+    }
+#else
     // We assume that doubles and uint64_t have the same endianness.
     static uint64_t double_to_uint64(double d) { return BitCast<uint64_t>(d); }
     static double uint64_to_double(uint64_t d64) { return BitCast<double>(d64); }
+#endif
     
     // Helper functions for doubles.
     class Double {
