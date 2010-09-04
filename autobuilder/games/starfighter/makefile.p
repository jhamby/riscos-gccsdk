--- makefile.orig	2010-09-04 14:10:32.000000000 +0100
+++ makefile	2010-09-04 14:16:19.000000000 +0100
@@ -1,5 +1,5 @@
 CFLAGS = `sdl-config --cflags` -Wall -DLINUX
-LIBS = `sdl-config --libs` -lSDL_mixer -lSDL_image
+LIBS = `sdl-config --libs` -lSDL_mixer -lSDL_image -lpng -ljpeg -lz -lvorbisidec
 OBJS = ai.o aliens.o audio.o bullets.o cargo.o collectable.o comms.o debris.o events.o explosions.o game.o globals.o graphics.o init.o intermission.o loadSave.o messages.o misc.o missions.o player.o resources.o script.o shop.o Starfighter.o title.o unpack.o weapons.o
 
 VERSION = 1.1
@@ -8,7 +8,7 @@
 DOCS = docs/*
 
 BINDIR = /usr/games/
-DATADIR = /usr/share/games/parallelrealities/
+DATADIR = /\<StarFighter\$$Dir\>/
 DOCDIR = /usr/share/doc/starfighter/
 # top-level rule to create the program.
 all: $(PROG)
@@ -19,7 +19,7 @@
 
 # linking the program.
 $(PROG): $(OBJS)
-	$(CXX) $(LIBS) $(OBJS) -o $(PROG)
+	$(CXX) -static $(LIBS) $(OBJS) -o $(PROG)
 
 # cleaning everything that can be automatically recreated with "make".
 clean:
