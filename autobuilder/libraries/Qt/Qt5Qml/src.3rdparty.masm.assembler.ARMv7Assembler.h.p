--- src/3rdparty/masm/assembler/ARMv7Assembler.h.orig	2021-08-02 14:04:48.317047733 -0700
+++ src/3rdparty/masm/assembler/ARMv7Assembler.h	2021-08-02 14:05:18.429116762 -0700
@@ -39,6 +39,10 @@
 #include <libkern/OSCacheControl.h>
 #endif
 
+#if OS(RISCOS)
+#include <swis.h>
+#endif
+
 namespace JSC {
 
 namespace ARMRegisters {
@@ -2311,6 +2315,8 @@
         UNUSED_PARAM(code);
         UNUSED_PARAM(size);
 #endif
+#elif OS(RISCOS)
+        _swix(OS_MMUControl, _IN(0), 0x80000001);
 #else
 #error "The cacheFlush support is missing on this platform."
 #endif
