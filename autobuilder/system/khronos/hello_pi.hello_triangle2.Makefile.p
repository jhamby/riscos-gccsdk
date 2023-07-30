--- host_applications/linux/apps/hello_pi/hello_triangle2/Makefile.orig	2022-11-23 17:47:07.056363390 +0000
+++ host_applications/linux/apps/hello_pi/hello_triangle2/Makefile	2022-11-23 17:47:07.176365152 +0000
@@ -1,5 +1,5 @@
 OBJS=triangle2.o
-BIN=hello_triangle2.bin
+BIN=hello_triangle2,e1f
 
 LDFLAGS+= -lrevision
 
