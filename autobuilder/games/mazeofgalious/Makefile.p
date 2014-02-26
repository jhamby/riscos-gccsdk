--- Makefile.orig	2010-01-15 11:35:01.000000000 -0800
+++ Makefile	2010-01-15 11:36:56.000000000 -0800
@@ -7,7 +7,7 @@
 all: mog
 
 %.o: %.cpp
-	c++ -c -g3 -O0 $< -o $@ `sdl-config --cflags` -I/usr/local/include/SDL
+	c++ -c -O3 $< -o $@ `sdl-config --cflags` -I/usr/local/include/SDL
 
 # dynamically linked binary:
 mog: $(OBJS)
@@ -15,7 +15,7 @@
 
 # static binary:
 mogs: $(OBJS)
-	c++ -static $^ -o $@ -lSDL_image -lSDL_mixer -lSDL_sound -lpng -ljpeg -lz `sdl-config --static-libs`
+	c++ -static $^ -o $@ -lSDL_image -lSDL_mixer -lSDL_sound -lpng -ljpeg -lz `sdl-config --static-libs` -logg -lvorbis -lvorbisfile -lmikmod -lFLAC
 
 clean:
 	rm -f mog mogs
