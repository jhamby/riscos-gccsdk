--- src/Makefile.orig	2013-06-02 19:47:25.000000000 +0100
+++ src/Makefile	2013-06-02 19:53:13.888459313 +0100
@@ -5,7 +5,7 @@
 OBJECTS:= $(CFILES:.c=.o)

 airstrike: $(OBJECTS)
-	$(CC) -o $@ $(CFLAGS) $(OBJECTS) `sdl-config --libs` -lSDL_image -lm
+	$(CC) -o $@ $(CFLAGS) $(OBJECTS) `sdl-config --libs` -lSDL_image -lm -ltiff -ljpeg -lpng -lz -static

 airstrike-sound: $(OBJECTS)
 	$(CC) -o $@ $(CFLAGS) $(OBJECTS) `sdl-config --libs` -lSDL_image -lSDL_mixer
