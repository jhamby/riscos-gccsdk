--- host_applications/linux/apps/hello_pi/hello_tiger/Makefile.orig	2015-06-18 20:04:26.000000000 +0100
+++ host_applications/linux/apps/hello_pi/hello_tiger/Makefile	2015-07-08 21:07:00.408090819 +0100
@@ -1,7 +1,7 @@
 OBJS=main.o tiger.o
-BIN=hello_tiger.bin
+BIN=hello_tiger,e1f
 
-#LDFLAGS+=
+LDFLAGS+=-lOpenVG
 CFLAGS+=-D__RASPBERRYPI__
 
 include ../Makefile.include
