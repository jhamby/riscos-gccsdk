--- lib/Support/PrettyStackTrace.cpp.orig	2014-04-09 05:20:00.000000000 +0100
+++ lib/Support/PrettyStackTrace.cpp	2015-02-05 20:43:37.180771479 +0000
@@ -26,6 +26,10 @@
 #include <CrashReporterClient.h>
 #endif
 
+#ifdef __riscos__
+#include <unixlib/local.h>
+#endif
+
 using namespace llvm;
 
 static ManagedStatic<sys::ThreadLocal<const PrettyStackTraceEntry> > PrettyStackTraceHead;
@@ -53,6 +57,10 @@
   
   PrintStack(PrettyStackTraceHead->get(), OS);
   OS.flush();
+
+#ifdef __riscos__
+  __write_backtrace(0);
+#endif
 }
 
 // Integrate with crash reporter libraries.
