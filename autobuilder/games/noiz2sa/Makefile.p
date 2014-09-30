--- src/Makefile.orig	2014-09-30 08:04:59.000000000 +0100
+++ src/Makefile	2014-09-30 08:17:31.000000000 +0100
@@ -7,16 +7,16 @@
 CC     = gcc
 CXX    = g++
 
-PROG        = $(NAME)
+PROG        = $(NAME)$(AB_EXEEXT)
 
 DEFAULT_CFLAGS = `sdl-config --cflags`
 LDFLAGS        = `sdl-config --libs` -lSDL_mixer -Lbulletml \
-    -lbulletml -lstdc++ -lm
+    -lbulletml -lstdc++ -lm -lmikmod -lvorbisidec -logg -static
 
 MORE_CFLAGS ?= -O3
 
 CFLAGS  = $(DEFAULT_CFLAGS) $(MORE_CFLAGS)
-CPPFLAGS  = $(DEFAULT_CFLAGS) $(MORE_CFLAGS) -I/usr/include
+CPPFLAGS  = $(DEFAULT_CFLAGS) $(MORE_CFLAGS) #-I/usr/include
 
 OBJS =	$(NAME).$(O) ship.$(O) shot.$(O) frag.$(O) bonus.$(O) \
 	foe.$(O) foecommand.$(O) barragemanager.$(O) attractmanager.$(O) \
