--- src/Makefile.orig	2021-02-13 09:48:47.095991545 +0000
+++ src/Makefile	2021-02-13 09:51:11.837061672 +0000
@@ -7,16 +7,16 @@
 CC     = gcc
 CXX    = g++
 
-PROG        = $(NAME)
+PROG        = $(NAME)$(AB_EXEEXT)
 
 DEFAULT_CFLAGS = `sdl-config --cflags`
 LDFLAGS        = `sdl-config --libs` -lSDL_mixer -Lbulletml \
-    -lbulletml -lstdc++ -lm
+    -lbulletml -lstdc++ -lm -lmikmod -lvorbisidec -logg -lFLAC -static
 
 MORE_CFLAGS ?= -O3
 
 CFLAGS  = $(DEFAULT_CFLAGS) $(MORE_CFLAGS)
-CPPFLAGS  = $(DEFAULT_CFLAGS) $(MORE_CFLAGS) -I/usr/include
+CPPFLAGS  = $(DEFAULT_CFLAGS) $(MORE_CFLAGS)
 
 OBJS =	$(NAME).$(O) ship.$(O) shot.$(O) frag.$(O) bonus.$(O) \
 	foe.$(O) foecommand.$(O) barragemanager.$(O) attractmanager.$(O) \
