--- src/Makefile.orig	2008-07-23 08:52:18.857534000 +0100
+++ src/Makefile	2008-07-23 10:37:55.204534000 +0100
@@ -8,12 +8,13 @@
 datadir=$(prefix)/share
 
 UI=sdl
-UILIBS=$(shell sdl-config --libs) -lSDL_mixer
+UILIBS=$(shell sdl-config --libs) -lSDL_mixer -lvorbisidec
 UIFLAGS=$(shell sdl-config --cflags)
 
-FEATURES=-DHAVE_BACKTRACE=1 -DHAVE_VARIADIC_MACROS=1
+FEATURES=-DHAVE_VARIADIC_MACROS=1
 DEBUGCXXFLAGS=-ggdb1 -rdynamic
-CXXFLAGS=$(FEATURES) -Wall -I. $(UIFLAGS) $(shell xml2-config --cflags) -DICON_FILE=\"$(datadir)/pixmaps/u4.bmp\" -DVERSION=\"$(VERSION)\" $(DEBUGCXXFLAGS)
+CXXFLAGS=$(FEATURES) -Wall -I. $(UIFLAGS) $(shell xml2-config --cflags) -DICON_FILE=\"$(datadir)/pixmaps/u4.bmp\" -DVERSION=\"$(VERSION)\" $(DEBUGCXXFLAGS) \
+ -O2
 CFLAGS=$(CXXFLAGS)
 LIBS=$(UILIBS) $(shell xml2-config --libs) -lpng
 INSTALL=install
@@ -21,6 +22,7 @@
 ifeq ($(STATIC_GCC_LIBS),true)
     LDFLAGS+=-L. -static-libgcc
 endif
+LDFLAGS+= -static -lz
 
 include Makefile.common
 
