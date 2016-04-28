--- Source/JavaScriptCore/runtime/JSCJSValueInlines.h.orig	2015-06-29 21:11:04.000000000 +0100
+++ Source/JavaScriptCore/runtime/JSCJSValueInlines.h	2016-04-03 11:42:15.364203104 +0100
@@ -290,7 +290,14 @@
 inline double JSValue::asDouble() const
 {
     ASSERT(isDouble());
+#if OS(RISCOS) && !(defined(__VFP_FP__) && !defined(__SOFTFP__))
+    EncodedValueDescriptor v;
+    v.asBits.tag = u.asBits.payload;
+    v.asBits.payload = u.asBits.tag;
+    return v.asDouble;
+#else
     return u.asDouble;
+#endif
 }
     
 ALWAYS_INLINE JSCell* JSValue::asCell() const
@@ -302,6 +309,11 @@
 ALWAYS_INLINE JSValue::JSValue(EncodeAsDoubleTag, double d)
 {
     u.asDouble = d;
+#if OS(RISCOS) && !(defined(__VFP_FP__) && !defined(__SOFTFP__))
+    uint32_t swap = u.asBits.tag;
+    u.asBits.tag = u.asBits.payload;
+    u.asBits.payload = swap;
+#endif
 }
 
 inline JSValue::JSValue(int i)
