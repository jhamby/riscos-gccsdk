--- sys/unix/Makefile.utl.orig	2003-12-07 15:39:13.000000000 -0800
+++ sys/unix/Makefile.utl	2009-08-27 10:34:11.000000000 -0700
@@ -89,8 +89,8 @@
 # flags for debugging:
 # CFLAGS = -g -I../include
 
-CFLAGS = -O -I../include
-LFLAGS =
+CFLAGS = -O2 -I../include $(GCCSDK_INSTALL_ENV)/include
+LFLAGS = -L$(GCCSDK_INSTALL_ENV)/lib
 
 LIBS =
  
