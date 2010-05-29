--- makefile.orig	2009-11-25 13:47:13.000000000 -0800
+++ makefile	2009-11-25 14:05:05.000000000 -0800
@@ -37,13 +37,13 @@
 CFLAGS = -Wall -g -pedantic -Werror -DVERSION=$(VERSION) -DRELEASE=$(RELEASE) -DDEV=$(DEV) -DINSTALL_PATH=\"$(DATA_DIR)\" -DLOCALE_DIR=\"$(LOCALE_DIR)\" -DPAK_FILE=\"$(PAK_FILE)\" -DMEDAL_SERVER_HOST=\"$(MEDAL_SERVER_HOST)\" -DMEDAL_SERVER_PORT=$(MEDAL_SERVER_PORT)
 #CFLAGS = -Wall -O2 -pedantic -DVERSION=$(VERSION) -DRELEASE=$(RELEASE) -DDEV=$(DEV) -DINSTALL_PATH=\"$(DATA_DIR)\" -DLOCALE_DIR=\"$(LOCALE_DIR)\" -DPAK_FILE=\"$(PAK_FILE)\"
 else
-CFLAGS = -Wall -Os -pedantic -DVERSION=$(VERSION) -DRELEASE=$(RELEASE) -DDEV=$(DEV) -DINSTALL_PATH=\"$(DATA_DIR)\" -DLOCALE_DIR=\"$(LOCALE_DIR)\" -DPAK_FILE=\"$(PAK_FILE)\" -DMEDAL_SERVER_HOST=\"$(MEDAL_SERVER_HOST)\" -DMEDAL_SERVER_PORT=$(MEDAL_SERVER_PORT)
+CFLAGS = -Wall -O2 -pedantic -DVERSION=$(VERSION) -DRELEASE=$(RELEASE) -DDEV=$(DEV) -DINSTALL_PATH=\"$(DATA_DIR)\" -DLOCALE_DIR=\"$(LOCALE_DIR)\" -DPAK_FILE=\"$(PAK_FILE)\" -DMEDAL_SERVER_HOST=\"$(MEDAL_SERVER_HOST)\" -DMEDAL_SERVER_PORT=$(MEDAL_SERVER_PORT)
 endif

 ifeq ($(OS),Windows_NT)
 LFLAGS = `sdl-config --libs` -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf -lSDL_net -lz -llibintl -lm
 else
-LFLAGS = `sdl-config --libs` -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf -lSDL_net -lz
+LFLAGS = -static `sdl-config --libs` -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf -lSDL_net -lz -lintl -lpng -ljpeg -lfreetype -lvorbisidec -lvorbisfile
 endif

 PAK_OBJS   = pak_creator.o
