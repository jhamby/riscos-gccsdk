--- Makefile.sdl.orig	2005-06-16 08:39:52.000000000 +0100
+++ Makefile.sdl	2005-06-16 08:39:58.000000000 +0100
@@ -24,7 +24,7 @@
 
 ifeq ($(ARCH),"LINUX")
 OBJS	+= fbsd_sup.o	# strup()
-CFLAGS	+= -DLINUX `sdl-config --cflags`
+CFLAGS	+= -DLINUX `sdl-config --cflags` -I/home/riscos/env/include
 LIBS	+= `sdl-config --libs` -lz
 ESUFFIX	=
 endif
