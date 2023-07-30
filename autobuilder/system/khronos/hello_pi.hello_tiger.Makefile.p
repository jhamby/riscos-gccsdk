--- host_applications/linux/apps/hello_pi/hello_tiger/Makefile.orig	2022-11-23 17:52:44.056647496 +0000
+++ host_applications/linux/apps/hello_pi/hello_tiger/Makefile	2022-11-23 18:28:33.981603866 +0000
@@ -1,7 +1,7 @@
 OBJS=main.o tiger.o
-BIN=hello_tiger.bin
+BIN=hello_tiger,e1f
 
-LDFLAGS+= -lrevision
+LDFLAGS+= -lOpenVG -lrevision
 CFLAGS+=-D__RASPBERRYPI__
 
 include ../Makefile.include
