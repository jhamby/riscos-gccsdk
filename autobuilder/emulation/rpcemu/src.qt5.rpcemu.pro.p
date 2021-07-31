--- src/qt5/rpcemu.pro.orig	2020-05-09 12:08:19.964898564 +0100
+++ src/qt5/rpcemu.pro	2020-05-09 12:08:39.880501171 +0100
@@ -3,7 +3,7 @@
 CONFIG += debug_and_release


-QT += core widgets gui multimedia
+QT += core widgets gui
 INCLUDEPATH += ../

 # This ensures that using switch with enum requires every value to be handled
@@ -30,7 +30,6 @@
 		configure_dialog.h \
 		about_dialog.h \
 		rpc-qt5.h \
-		plt_sound.h

 SOURCES =	../superio.c \
 		../cdrom-iso.c \
@@ -57,8 +56,7 @@
 		rpc-qt5.cpp \
 		main_window.cpp \
 		configure_dialog.cpp \
-		about_dialog.cpp \
-		plt_sound.cpp
+		about_dialog.cpp

 # NAT Networking
 linux | win32 {
