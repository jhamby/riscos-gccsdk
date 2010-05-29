--- Makefile.orig	2010-05-28 19:34:54.000000000 +0100
+++ Makefile	2010-05-28 19:35:06.000000000 +0100
@@ -2,7 +2,7 @@
 # Allan Crossman
 
 # Where the graphics and sound files go...
-DATAPATH=/usr/local/share/komidata/
+DATAPATH=/\<komi\$$dir\>/komidata/
 
 # Where to put the binary...
 BINPATH=/usr/local/bin/
@@ -10,7 +10,7 @@
 # Where to put the manpage...
 MANPATH=/usr/local/man/man6/
 
-MIXER=-lSDL_mixer
+MIXER=-lSDL_mixer -logg -lvorbis -lvorbisfile
 SDL_LIB=$(shell sdl-config --static-libs) $(MIXER)
 
 CFLAGS=-Wall -pedantic -std=c99 -O2 \
@@ -35,6 +35,6 @@
 	cp komi.6 $(MANPATH)komi.6
 
 komi:	komi.o
-	$(CC) $(CFLAGS) komi.o -o komi $(SDL_LIB)
+	$(CC) -static $(CFLAGS) komi.o -o komi $(SDL_LIB)
 
 komi.o:	komi.c
