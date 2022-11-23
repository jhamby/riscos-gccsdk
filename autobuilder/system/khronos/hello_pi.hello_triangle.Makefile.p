--- host_applications/linux/apps/hello_pi/hello_triangle/Makefile.orig	2022-11-23 17:47:07.052363331 +0000
+++ host_applications/linux/apps/hello_pi/hello_triangle/Makefile	2022-11-23 17:47:07.176365152 +0000
@@ -1,5 +1,5 @@
 OBJS=triangle.o
-BIN=hello_triangle.bin
+BIN=hello_triangle,e1f
 
 LDFLAGS+= -lrevision
 
