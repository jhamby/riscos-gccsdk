--- src/corelib/tools/tools.pri.orig	2015-02-17 04:56:49.000000000 +0000
+++ src/corelib/tools/tools.pri	2015-04-17 15:28:27.998780051 +0100
@@ -215,3 +215,8 @@
 MIPS_DSP_ASM += tools/qstring_mips_dsp_asm.S
 MIPS_DSP_HEADERS += ../gui/painting/qt_mips_asm_dsp_p.h
 CONFIG += simd
+
+*riscos* {
+    SOURCES += tools/qdynamicarea.cpp
+    HEADERS += tools/qdynamicarea.h
+}
