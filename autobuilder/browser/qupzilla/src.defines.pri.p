--- src/defines.pri.orig	2016-02-13 11:43:11.630423910 +0000
+++ src/defines.pri	2016-02-13 11:47:49.410422823 +0000
@@ -60,8 +60,14 @@
     DEFINES *= NO_X11
 }
 
+*riscos* {
+    DEFINES *= DISABLE_DBUS
+    DEFINES *= NO_SYSTEM_DATAPATH
+    DEFINES *= NO_X11
+}
+
 # Check for pkg-config availability
-!mac:unix:system(pkg-config --version > /dev/null) {
+!mac:!*riscos*:unix:system(pkg-config --version > /dev/null) {
     isEqual(QT_MAJOR_VERSION, 5) {
         MODNAME = Qt5WebKitWidgets
     }
@@ -102,7 +108,7 @@
 }
 
 
-!mac:unix {
+!mac:!*riscos*:unix {
     binary_folder = /usr/bin
     library_folder = /usr/lib
     arch_lib_path = /usr/lib/$${QT_ARCH}-linux-gnu
