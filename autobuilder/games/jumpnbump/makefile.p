--- Makefile.orig	2021-04-08 21:32:13.848063737 +0100
+++ Makefile	2021-04-08 21:34:44.437510609 +0100
@@ -3,7 +3,7 @@
 CFLAGS = -Wall -O2 -ffast-math -funroll-loops -Dstricmp=strcasecmp \
 	-Dstrnicmp=strncasecmp -DUSE_SDL -DNDEBUG -I. $(SDL_CFLAGS) \
 	-DUSE_NET -DZLIB_SUPPORT -DBZLIB_SUPPORT
-LIBS = -lm $(SDL_LIBS) -lSDL_mixer -lSDL_net -lbz2 -lz
+LIBS = -lm $(SDL_LIBS) -lSDL_mixer -lSDL_net -lbz2 -lz -lvorbisidec -lvorbisfile -logg -lmikmod -lFLAC -static
 SDL_TARGET = sdl.a
 MODIFY_TARGET = gobpack jnbpack jnbunpack
 OBJS = fireworks.o main.o menu.o filter.o
