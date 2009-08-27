--- makefile.orig	2009-08-13 03:33:47.000000000 -0700
+++ makefile	2009-08-27 11:15:04.000000000 -0700
@@ -13,9 +13,9 @@
 TARGET := $(TARGET).exe
 
 
-CC 		= g++
-CFLAGS 	=  -O2 -g -I/usr/local/include -L/usr/local/lib -DVERSION_NUM="\"$(VERSION)\""  
-LDFLAGS =  
+CC 		= $(CXX)
+CFLAGS 	=  -O2 -g -I$(GCCSDK_INSTALL_ENV)/include -DVERSION_NUM="\"$(VERSION)\""  
+LDFLAGS =  -L$(GCCSDK_INSTALL_ENV)/lib -static
 
 OBJS = apertures.o main.o  gerber.o polygon.o lex.yy.o gerber.tab.o 
 
