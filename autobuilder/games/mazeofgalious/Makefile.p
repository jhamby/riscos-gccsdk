--- Makefile.orig	2009-12-02 11:50:05.000000000 -0800
+++ Makefile	2009-12-02 12:46:17.000000000 -0800
@@ -7,22 +7,23 @@
 all: mog
 
 %.o: %.cpp
-	c++ -c -g3 -O0 $< -o $@ `sdl-config --cflags` -I/usr/local/include/SDL
+	$(CXX) -c -O3 $< -o $@ `sdl-config --cflags` -I$(GCCSDK_INSTALL_ENV)/include
 
 # dynamically linked binary:
 mog: $(OBJS)
-	c++ $^ -o $@ `sdl-config --libs` -lSDL_image -lSDL_mixer -lSDL_sound -lSDL_sound -I/usr/local/include/SDL
+	$(CXX) $^ -o $@ `sdl-config --libs` -lSDL_image -lSDL_mixer -lSDL_sound -lSDL_sound -I/usr/local/include/SDL
 
 # static binary:
 mogs: $(OBJS)
-	c++ -static $^ -o $@ -lSDL_image -lSDL_mixer -lSDL_sound -lpng -ljpeg -lz `sdl-config --static-libs`
+	$(CXX) -static $^ -o $@ -lSDL_image -lSDL_mixer -lSDL_sound -lpng -ljpeg -lz `sdl-config --static-libs` \
+	-logg -lvorbis -lvorbisfile -lFLAC
 
 clean:
 	rm -f mog mogs
-	rm -f *.o
-
-install:
-	cp mog $(DESTDIR)/usr/games
-
-install-data:
-	cp sound rooms graphics -r $(DESTDIR)/usr/share/games/mazeofgalious/
+	rm -f *.o
+
+install:
+	cp mog $(DESTDIR)/usr/games
+
+install-data:
+	cp sound rooms graphics -r $(DESTDIR)/usr/share/games/mazeofgalious/
