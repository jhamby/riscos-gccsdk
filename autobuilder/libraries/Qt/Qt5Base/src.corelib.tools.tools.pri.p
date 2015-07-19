--- src/corelib/tools/tools.pri.orig	2015-06-29 21:05:06.000000000 +0100
+++ src/corelib/tools/tools.pri	2015-07-15 18:24:19.886735896 +0100
@@ -208,3 +208,8 @@
 MIPS_DSP_ASM += tools/qstring_mips_dsp_asm.S
 MIPS_DSP_HEADERS += ../gui/painting/qt_mips_asm_dsp_p.h
 CONFIG += simd
+
+*riscos* {
+    SOURCES += tools/qdynamicarea.cpp
+    HEADERS += tools/qdynamicarea.h
+}
