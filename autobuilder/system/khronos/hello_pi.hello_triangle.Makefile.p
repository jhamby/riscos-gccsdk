--- host_applications/linux/apps/hello_pi/hello_triangle/Makefile.orig	2015-06-18 20:04:26.000000000 +0100
+++ host_applications/linux/apps/hello_pi/hello_triangle/Makefile	2015-06-21 15:03:52.295998495 +0100
@@ -1,5 +1,5 @@
 OBJS=triangle.o
-BIN=hello_triangle.bin
+BIN=hello_triangle,e1f
 
 include ../Makefile.include
 
