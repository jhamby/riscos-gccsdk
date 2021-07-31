--- Source/JavaScriptCore/interpreter/JSStackInlines.h.orig	2014-05-15 18:12:34.000000000 +0100
+++ Source/JavaScriptCore/interpreter/JSStackInlines.h	2014-08-02 14:05:00.577867624 +0100
@@ -129,7 +129,13 @@
 #if ENABLE(DEBUG_JSSTACK)
 inline JSValue JSStack::generateFenceValue(size_t argIndex)
 {
+#if OS(RISCOS)
+    // Avoid fenceBits being interpreted as a negative integer number and
+    // converted to a double in JSValue::JSValue(unsigned).
+    unsigned fenceBits = 0x0acebad0 | ((argIndex+1) & 0xf);
+#else
     unsigned fenceBits = 0xfacebad0 | ((argIndex+1) & 0xf);
+#endif
     JSValue fenceValue = JSValue(fenceBits);
     return fenceValue;
 }
