--- src/CMakeLists.txt.orig	2010-07-13 20:57:53.000000000 +0100
+++ src/CMakeLists.txt	2011-06-09 14:05:44.516255800 +0100
@@ -74,6 +74,13 @@
     ${LIBDBUS_LIBRARIES}
 )
 
+# Extra libraries for static RISC OS build
+set( game-external-libs
+     ${game-external-libs}
+     vorbisidec
+     jpeg
+)
+
 if(MSVC)
 	set( game-external-libs
 		${game-external-libs}
@@ -90,6 +97,14 @@
     ${SDLNET_LIBRARY}
 )
 
+# Extra libraries for static RISC OS build
+set( server-external-libs
+     ${server-external-libs}
+     z
+     intl
+)
+
+
 set( tools-external-libs
     ${common-external-libs}
     ${SDLIMAGE_LIBRARY}
