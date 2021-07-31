--- Source/WTF/WTF.pri.orig	2015-02-17 04:57:01.000000000 +0000
+++ Source/WTF/WTF.pri	2015-04-16 15:18:10.745589539 +0100
@@ -30,6 +30,8 @@
     else: LIBS += -licui18n -licuuc -licudata
 }
 
+arm-riscos*-g++*:LIBS += -licui18n -licuuc -licudata
+
 use?(GLIB) {
     PKGCONFIG *= glib-2.0 gio-2.0
 }
