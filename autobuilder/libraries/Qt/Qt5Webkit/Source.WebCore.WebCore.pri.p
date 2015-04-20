--- Source/WebCore/WebCore.pri.orig	2014-05-15 18:12:25.000000000 +0100
+++ Source/WebCore/WebCore.pri	2014-06-18 21:22:37.800688652 +0100
@@ -304,6 +304,11 @@
 unix:!mac:*-g++*:QMAKE_LFLAGS += -Wl,--gc-sections
 linux*-g++*:QMAKE_LFLAGS += $$QMAKE_LFLAGS_NOUNDEF
 
+#arm-riscos*-g++*:QMAKE_CXXFLAGS += -mno-poke-function-name
+# --gc-sections causes linker error "error: required section '.riscos.abi.version' not found in the linker script"
+arm*-riscos*-*:QMAKE_LFLAGS -= -Wl,--gc-sections
+arm*-riscos*-*:QMAKE_LFLAGS += -Wl,--no-undefined
+
 enable_fast_mobile_scrolling: DEFINES += ENABLE_FAST_MOBILE_SCROLLING=1
 
 build?(qttestsupport):have?(FONTCONFIG): PKGCONFIG += fontconfig
