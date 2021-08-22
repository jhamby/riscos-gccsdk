--- src/corelib/tools/tools.pri.orig	2021-08-16 13:20:34.209132858 -0700
+++ src/corelib/tools/tools.pri	2021-08-16 13:33:33.554586099 -0700
@@ -227,3 +227,8 @@
 # MIPS DSP
 MIPS_DSP_ASM += tools/qstring_mips_dsp_asm.S
 MIPS_DSP_HEADERS += ../gui/painting/qt_mips_asm_dsp_p.h
+
+*riscos* {
+    SOURCES += tools/qdynamicarea.cpp
+    HEADERS += tools/qdynamicarea.h
+}
