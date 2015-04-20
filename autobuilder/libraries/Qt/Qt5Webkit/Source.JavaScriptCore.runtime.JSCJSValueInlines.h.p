--- Source/JavaScriptCore/runtime/JSCJSValueInlines.h.orig	2014-05-15 18:12:35.000000000 +0100
+++ Source/JavaScriptCore/runtime/JSCJSValueInlines.h	2014-08-25 21:02:56.271845684 +0100
@@ -290,7 +290,14 @@
 inline double JSValue::asDouble() const
 {
     ASSERT(isDouble());
+#if OS(RISCOS)
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
+#if OS(RISCOS)
+    uint32_t swap = u.asBits.tag;
+    u.asBits.tag = u.asBits.payload;
+    u.asBits.payload = swap;
+#endif
 }
 
 inline JSValue::JSValue(int i)
