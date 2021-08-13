--- host_applications/linux/apps/hello_pi/hello_tiger/Makefile.orig	2021-07-28 19:42:05.140204301 -0700
+++ host_applications/linux/apps/hello_pi/hello_tiger/Makefile	2021-07-28 19:45:52.369425954 -0700
@@ -1,7 +1,7 @@
 OBJS=main.o tiger.o
-BIN=hello_tiger.bin
+BIN=hello_tiger,e1f
 
-LDFLAGS+= -lrevision
+LDFLAGS+= -lOpenVG -L../lib/revision -lrevision
 CFLAGS+=-D__RASPBERRYPI__
 
 include ../Makefile.include
