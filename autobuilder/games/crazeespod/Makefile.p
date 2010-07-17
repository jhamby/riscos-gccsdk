--- Makefile.orig	2010-07-16 17:29:54.000000000 +0100
+++ Makefile	2010-07-16 17:30:27.000000000 +0100
@@ -26,7 +26,7 @@
 	${RM} $(OBJ) $(BIN)
 
 $(BIN): $(OBJ)
-	$(CC) $(LINKOBJ) -o "./CrazeeSpod" $(LIBS)
+	$(CC) -static $(LINKOBJ) -o "./CrazeeSpod" $(LIBS) -lpng -ljpeg -lfreetype -lvorbisidec -lz
 
 
 src/images.o: src/images.c
