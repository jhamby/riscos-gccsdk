--- Source/JavaScriptCore/jit/JITStubs.h.orig	2015-06-29 21:11:07.000000000 +0100
+++ Source/JavaScriptCore/jit/JITStubs.h	2016-01-24 19:25:07.369370465 +0000
@@ -208,6 +208,16 @@
 
     ReturnAddressPtr thunkReturnAddress;
 
+#if OS(RISCOS)
+    void* preservedR4;
+    void* preservedR5;
+    void* preservedR6;
+    void* preservedR7;
+    void* preservedR9;
+    void* preservedR10;
+    void* preservedR11;
+    void* preservedLink;
+#else
     void* preservedR4;
     void* preservedR5;
     void* preservedR6;
@@ -216,6 +226,7 @@
     void* preservedR10;
     void* preservedR11;
     void* preservedLink;
+#endif
 
     JSStack* stack;
     CallFrame* callFrame;
