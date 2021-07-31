--- Makefile.orig	2010-07-16 12:20:24.000000000 +0100
+++ Makefile	2010-07-21 21:06:12.000000000 +0100
@@ -12,7 +12,7 @@
 #'pkg-config --cflags librsvg-2.0' 'pkg-config --libs librsvg-2.0'
 
 BIN  = ./CrazeeSpod
-CFLAGS= -Wall  -DLGFX
+CFLAGS= -Wall
 
 
 RM = rm -f
@@ -26,7 +26,7 @@
 	${RM} $(OBJ) $(BIN)
 
 $(BIN): $(OBJ)
-	$(CC) $(LINKOBJ) -o "./CrazeeSpod" $(LIBS)
+	$(CC) -static $(LINKOBJ) -o "./CrazeeSpod" $(LIBS) -lpng -ljpeg -lfreetype -lvorbisidec -lz
 
 
 src/images.o: src/images.c
