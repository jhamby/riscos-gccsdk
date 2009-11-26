--- makefile.orig	2009-11-25 13:47:13.000000000 -0800
+++ makefile	2009-11-25 14:05:05.000000000 -0800
@@ -17,7 +17,7 @@
 PO_PROG   = po_creator
 endif
 
-CXX       = gcc
+#CXX       = gcc
 
 PREFIX = $(DESTDIR)/usr
 BIN_DIR = $(PREFIX)/games/
@@ -37,13 +37,13 @@
 CFLAGS = -Wall -g -pedantic -Werror -DVERSION=$(VERSION) -DRELEASE=$(RELEASE) -DDEV=$(DEV) -DINSTALL_PATH=\"$(DATA_DIR)\" -DLOCALE_DIR=\"$(LOCALE_DIR)\" -DPAK_FILE=\"$(PAK_FILE)\"
 #CFLAGS = -Wall -O2 -pedantic -DVERSION=$(VERSION) -DRELEASE=$(RELEASE) -DDEV=$(DEV) -DINSTALL_PATH=\"$(DATA_DIR)\" -DLOCALE_DIR=\"$(LOCALE_DIR)\" -DPAK_FILE=\"$(PAK_FILE)\"
 else
-CFLAGS = -Wall -O2 -pedantic -DVERSION=$(VERSION) -DRELEASE=$(RELEASE) -DDEV=$(DEV) -DINSTALL_PATH=\"$(DATA_DIR)\" -DLOCALE_DIR=\"$(LOCALE_DIR)\" -DPAK_FILE=\"$(PAK_FILE)\"
+CFLAGS = -Wall -O2 -pedantic -DVERSION=$(VERSION) -DRELEASE=$(RELEASE) -DDEV=$(DEV) -DINSTALL_PATH=\"$(DATA_DIR)\" -DLOCALE_DIR=\"$(LOCALE_DIR)\" -DPAK_FILE=\"$(PAK_FILE)\" -I$(GCCSDK_INSTALL_ENV)/include
 endif
 
 ifeq ($(OS),Windows_NT)
 LFLAGS = `sdl-config --libs` -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf -lz -llibintl -lm
 else
-LFLAGS = `sdl-config --libs` -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf -lz
+LFLAGS = -static -L$(GCCSDK_INSTALL_ENV)/lib  `sdl-config --libs` -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf -lz -lintl -lpng -lfreetype -lvorbisidec -lvorbisfile
 endif
 
 PAK_OBJS   = pak_creator.o
