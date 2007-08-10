--- Makefile.orig	2007-08-08 13:07:06.400313900 +0100
+++ Makefile	2007-08-08 13:09:23.749313900 +0100
@@ -2,7 +2,8 @@
 # Allan Crossman
 
 # Where the graphics and sound files go...
-DATAPATH=/usr/local/share/komidata/
+#DATAPATH=/usr/local/share/komidata/
+DATAPATH=/\<komi\$$dir\>/komidata/
 
 # Where to put the binary...
 BINPATH=/usr/local/bin/
@@ -10,7 +11,7 @@
 # Where to put the manpage...
 MANPATH=/usr/local/man/man6/
 
-MIXER=-lSDL_mixer
+MIXER=-lSDL_mixer -logg -lvorbis -lvorbisfile
 SDL_LIB=$(shell sdl-config --static-libs) $(MIXER)
 
 CFLAGS=-Wall -pedantic -std=c99 -O2 \
