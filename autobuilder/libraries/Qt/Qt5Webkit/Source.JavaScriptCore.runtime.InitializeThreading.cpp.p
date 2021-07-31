--- Source/JavaScriptCore/runtime/InitializeThreading.cpp.orig	2014-07-06 16:03:57.000000000 +0100
+++ Source/JavaScriptCore/runtime/InitializeThreading.cpp	2014-07-06 16:06:22.808838430 +0100
@@ -43,6 +43,10 @@
 #include <wtf/Threading.h>
 #include <wtf/dtoa/cached-powers.h>
 
+#if OS(RISCOS)
+#include "wtf/Riscos.h"
+#endif
+
 using namespace WTF;
 
 namespace JSC {
@@ -69,6 +73,9 @@
 #if ENABLE(LLINT)
     LLInt::initialize();
 #endif
+#if OS(RISCOS)
+    riscos::installBreakpointHandler();
+#endif
 }
 
 void initializeThreading()
