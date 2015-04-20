--- Source/WTF/wtf/Assertions.cpp.orig	2014-05-15 18:12:23.000000000 +0100
+++ Source/WTF/wtf/Assertions.cpp	2014-06-20 20:31:44.238078845 +0100
@@ -75,6 +75,10 @@
 #include <BlackBerryPlatformLog.h>
 #endif
 
+#if OS(RISCOS)
+#include <unixlib/local.h>
+#endif
+
 extern "C" {
 
 WTF_ATTRIBUTE_PRINTF(1, 0)
@@ -291,6 +295,9 @@
 
 void WTFPrintBacktrace(void** stack, int size)
 {
+#if OS(RISCOS)
+    __write_backtrace (0);
+#endif
 #if USE(BACKTRACE_SYMBOLS)
     char** symbols = backtrace_symbols(stack, size);
     if (!symbols)
