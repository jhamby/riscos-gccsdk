--- src/Makefile.orig	2007-08-01 08:47:51.354918600 +0100
+++ src/Makefile	2007-08-01 08:48:34.100918600 +0100
@@ -5,7 +5,7 @@
 OBJECTS:= $(CFILES:.c=.o)
 
 airstrike: $(OBJECTS)
-	$(CC) -o $@ $(CFLAGS) $(OBJECTS) `sdl-config --libs` -lSDL_image
+	$(CC) -o $@ $(CFLAGS) $(OBJECTS) `sdl-config --libs` -lSDL_image -ltiff -ljpeg -lpng -lz -static
 
 airstrike-sound: $(OBJECTS)
 	$(CC) -o $@ $(CFLAGS) $(OBJECTS) `sdl-config --libs` -lSDL_image -lSDL_mixer
