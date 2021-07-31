--- makefile.orig	2011-04-17 14:56:56.000000000 +0000
+++ makefile	2021-03-06 13:19:18.579852803 +0000
@@ -10,7 +10,7 @@
 
 PREFIX ?= /usr
 BINDIR ?= $(PREFIX)/games/
-DATADIR ?= $(PREFIX)/share/games/blobwars/
+DATADIR ?= "/<Blobwars\$$Dir>/data/"
 DOCDIR ?= $(PREFIX)/share/doc/$(PROG)/
 ICONDIR ?= $(PREFIX)/share/icons/hicolor/
 DESKTOPDIR ?= $(PREFIX)/share/applications/
@@ -21,8 +21,8 @@
 CXXFLAGS += `sdl-config --cflags` -DVERSION=$(VERSION) -DRELEASE=$(RELEASE) -DUSEPAK=$(USEPAK)
 CXXFLAGS += -DPAKNAME=\"$(PAKNAME)\" -DPAKLOCATION=\"$(DATADIR)\" -DUNIX -DGAMEPLAYMANUAL=\"$(DOCDIR)index.html\" -Wall
 CXXFLAGS += -DLOCALEDIR=\"$(LOCALEDIR)\" -DMEDAL_SERVER_HOST=\"$(MEDAL_SERVER_HOST)\" -DMEDAL_SERVER_PORT=$(MEDAL_SERVER_PORT)
-CXXFLAGS += $(CFLAGS) -Werror
-LIBS = `sdl-config --libs` -lSDL_mixer -lSDL_image -lSDL_ttf -lSDL_net -lz
+CXXFLAGS += $(CFLAGS)
+LIBS = -static `sdl-config --libs` -lSDL -lSDL_mixer -lSDL_image -lSDL_mixer -lSDL_ttf -lSDL_net -lz -lintl -lpng -ljpeg -lfreetype -lvorbisidec -lvorbisfile -logg -lbz2 -lFLAC -lwebp -lmikmod
 
 OBJS += CAudio.o
 OBJS += CBoss.o
