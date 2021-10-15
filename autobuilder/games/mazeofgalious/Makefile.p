--- Makefile.orig	2021-10-15 09:59:39.677679216 +0100
+++ Makefile	2021-10-15 10:03:44.284963425 +0100
@@ -7,7 +7,7 @@
 all: mog
 
 %.o: %.cpp
-	c++ -c $(CFLAGS) $(CPPFLAGS) $< -o $@ `sdl-config --cflags`
+	g++ -c $(CFLAGS) $(CPPFLAGS) $< -o $@ `sdl-config --cflags`
 
 # dynamically linked binary:
 mog: $(OBJS)
@@ -15,7 +15,7 @@
 
 # static binary:
 mogs: $(OBJS)
-	c++ -static $^ -o $@ -lSDL_image -lSDL_mixer -lSDL_sound -lpng -ljpeg -lz `sdl-config --static-libs`
+	gcc -static $^ -o $@ -lSDL_image -lpng -ljpeg -lz `sdl-config --static-libs` -logg -lvorbis -lvorbisfile -lmikmod -lFLAC -lspeex -lstdc++ -ltiff -lwebp -llzma -lSDL_sound -lSDL_mixer
 
 clean:
 	rm -f mog mogs
