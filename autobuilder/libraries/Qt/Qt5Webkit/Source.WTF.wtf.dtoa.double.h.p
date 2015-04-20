--- Source/WTF/wtf/dtoa/double.h.orig	2014-05-15 18:12:23.000000000 +0100
+++ Source/WTF/wtf/dtoa/double.h	2014-08-01 20:47:24.042770811 +0100
@@ -35,8 +35,23 @@
 namespace double_conversion {
     
     // We assume that doubles and uint64_t have the same endianness.
+#if OS(RISCOS)
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
     static uint64_t double_to_uint64(double d) { return BitCast<uint64_t>(d); }
     static double uint64_to_double(uint64_t d64) { return BitCast<double>(d64); }
+#endif
     
     // Helper functions for doubles.
     class Double {
