--- Source/JavaScriptCore/jsc.cpp.orig	2014-05-15 18:12:36.000000000 +0100
+++ Source/JavaScriptCore/jsc.cpp	2014-06-23 20:56:28.391329357 +0100
@@ -94,6 +94,15 @@
 #include <Ecore.h>
 #endif
 
+#if OS(RISCOS)
+#include <features.h>
+
+// Override the __stack_size weak symbol in Unixlib.
+// Force it to be visible incase all symbols are hidden by default in
+// build system.
+int __stack_size __attribute__ ((visibility ("default"))) = 1024 * 1024; // 1MB stack
+#endif
+
 using namespace JSC;
 using namespace WTF;
 
