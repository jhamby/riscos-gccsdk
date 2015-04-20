--- Source/JavaScriptCore/disassembler/ARMv7/ARMv7DOpcode.cpp.orig	2014-05-15 18:12:36.000000000 +0100
+++ Source/JavaScriptCore/disassembler/ARMv7/ARMv7DOpcode.cpp	2014-07-10 13:46:51.382451335 +0100
@@ -32,6 +32,7 @@
 #include <stdarg.h>
 #include <stdint.h>
 #include <stdio.h>
+#include <string.h>
 
 namespace JSC { namespace ARMv7Disassembler {
 
