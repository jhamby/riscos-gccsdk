--- makefile.orig	2021-03-07 14:15:32.742908456 +0000
+++ makefile	2021-03-07 14:23:47.046068830 +0000
@@ -1,6 +1,6 @@
 CFLAGS ?= -O2 -Wall -g
 CFLAGS += `sdl-config --cflags` -DLINUX
-LIBS = `sdl-config --libs` -lSDL_mixer -lSDL_image
+LIBS = `sdl-config --libs` -lSDL_mixer -lSDL_image -lpng -ljpeg -lz -lvorbisidec -logg -lFLAC -lmikmod -lwebp -ltiff -llzma
 OBJS = ai.o aliens.o audio.o bullets.o cargo.o collectable.o comms.o debris.o events.o explosions.o game.o globals.o graphics.o init.o intermission.o loadSave.o messages.o misc.o missions.o player.o resources.o script.o shop.o Starfighter.o title.o unpack.o weapons.o
 
 VERSION = 1.2
@@ -14,7 +14,7 @@
 
 PREFIX ?= /usr
 BINDIR ?= $(PREFIX)/games/
-DATADIR ?= $(PREFIX)/share/games/parallelrealities/
+DATADIR ?= /\<StarFighter\$$Dir\>/
 DOCDIR ?= $(PREFIX)/share/doc/$(PROG)/
 
 # top-level rule to create the program.
@@ -31,7 +31,7 @@
 
 # linking the program.
 $(PROG): $(OBJS)
-	$(CXX) $(LDFLAGS) $(OBJS) -o $(PROG) $(LIBS)
+	$(CXX) -static $(LDFLAGS) $(OBJS) -o $(PROG) $(LIBS)
 
 # cleaning everything that can be automatically recreated with "make".
 clean:
