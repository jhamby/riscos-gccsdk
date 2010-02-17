--- Makefile.orig	2010-02-15 22:11:09.000000000 +0000
+++ Makefile	2010-02-15 22:16:52.000000000 +0000
@@ -12,7 +12,7 @@
 DEBUG_MODE=false
 
 # Unix/Linux settings
-PREFIX=/usr
+PREFIX=$(GCCSDK_INSTALL_ENV)
 DATADIR=$(PREFIX)/share/games/flobopuyo
 INSTALL_BINDIR=$(DESTDIR)/$(PREFIX)/games
 INSTALL_DATADIR=$(DESTDIR)/$(DATADIR)
@@ -41,15 +41,15 @@
 ##############
 
 SDL_CONFIG=sdl-config
-CC=g++
-CXX=g++
+CC=g++
+CXX=g++
 
-CFLAGS= -DDATADIR=\"${DATADIR}\"
+CFLAGS= -DDATADIR=\"/\<Flobopuyo\$$Dir\>/data\"
 LDFLAGS=
 
 ifneq ($(PLATFORM), $(CYGWIN_VERSION))
 CFLAGS:=$(CFLAGS) `$(SDL_CONFIG) --cflags` -I/sw/include
-LDFLAGS:=$(LDFLAGS) `$(SDL_CONFIG) --cflags --libs`
+LDFLAGS:=$(LDFLAGS) -static `$(SDL_CONFIG) --cflags --libs` -ljpeg -lpng -lz -logg -lvorbis -lvorbisfile
 endif
 
 HFILES= HiScores.h IosException.h IosImgProcess.h IosVector.h PuyoCommander.h\
