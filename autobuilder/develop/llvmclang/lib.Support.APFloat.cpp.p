--- lib/Support/APFloat.cpp.orig	2014-06-08 17:53:31.000000000 +0100
+++ lib/Support/APFloat.cpp	2015-01-11 14:30:41.178077324 +0000
@@ -2999,9 +2999,20 @@
     mysignificand = *significandParts();
   }
 
+#if 1
+  {
+    // RISCOS: Swap the two 4 byte words.
+    // FIXME: Unfortunately there's no target info to make this RISC OS specific.
+    uint64_t result = (((uint64_t)(sign & 1) << 63) |
+                     ((myexponent & 0x7ff) <<  52) |
+                     (mysignificand & 0xfffffffffffffLL));
+    return APInt(64, Make_64(Lo_32(result), Hi_32(result)));
+  }
+#else
   return APInt(64, ((((uint64_t)(sign & 1) << 63) |
                      ((myexponent & 0x7ff) <<  52) |
                      (mysignificand & 0xfffffffffffffLL))));
+#endif
 }
 
 APInt
@@ -3104,6 +3115,10 @@
   assert(semantics == (const llvm::fltSemantics*)&IEEEdouble &&
          "Float semantics are not IEEEdouble");
   APInt api = bitcastToAPInt();
+
+  // RISCOS: bitcastToAPInt() above has already swapped the two 4 byte words
+  // for a 64bit double, so no need to do it again here.
+
   return api.bitsToDouble();
 }
 
@@ -3219,6 +3234,15 @@
 {
   assert(api.getBitWidth()==64);
   uint64_t i = *api.getRawData();
+  
+#if 1
+  {
+    // RISCOS: Swap the two 4 byte words.
+    // FIXME: Unfortunately there's no target info to make this RISC OS specific.
+    i = Make_64(Lo_32(i), Hi_32(i));
+  }
+#endif
+
   uint64_t myexponent = (i >> 52) & 0x7ff;
   uint64_t mysignificand = i & 0xfffffffffffffLL;
 
