--- makefile.orig	2009-11-23 09:45:11.000000000 -0800
+++ makefile	2009-11-23 09:45:23.000000000 -0800
@@ -13,9 +13,9 @@
 TARGET := $(TARGET).exe
 
 
-CC 		= g++
-CFLAGS 	=  -O2 -g -I/usr/local/include -L/usr/local/lib -DVERSION_NUM="\"$(VERSION)\""  
-LDFLAGS =  
+CC 		= $(CXX)
+CFLAGS 	=  -O2 -g -I$(GCCSDK_INSTALL_ENV)/include -DVERSION_NUM="\"$(VERSION)\""  
+LDFLAGS =  -L$(GCCSDK_INSTALL_ENV)/lib -static
 
 OBJS = apertures.o main.o  gerber.o polygon.o lex.yy.o gerber.tab.o 
 
@@ -26,7 +26,7 @@
 
 $(TARGET) : $(OBJS)
 	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -static -ltiff -lz -ljpeg -o $(TARGET)   
-	cp $(TARGET) /usr/local/bin
+	#cp $(TARGET) /usr/local/bin
 
 version:
 	@echo +---------------------------------+
