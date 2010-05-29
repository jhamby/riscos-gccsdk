--- src/bulletml/Makefile.orig	2010-05-28 10:15:36.000000000 +0100
+++ src/bulletml/Makefile	2010-05-28 10:16:47.000000000 +0100
@@ -1,9 +1,9 @@
 CC = g++
-INCLUDES = -I/usr/include
+INCLUDES =
 LIBS = tinyxml/tinyxml.o tinyxml/tinyxmlparser.o tinyxml/tinyxmlerror.o
 #CXXFLAGS = -g -W -Wall -ansi -pedantic
 #CXXFLAGS = -pg -g -W -Wall -ansi -pedantic
-CXXFLAGS ?= -O2 -g -Wall
+CXXFLAGS ?= -O2 -g -Wall -I.
 CFLAGS ?= -O2
 OBJS = bulletmlparser-tinyxml.o bulletmlparser.o bulletmltree.o calc.o formula-variables.o bulletmlrunner.o bulletmlrunnerimpl.o

--- src/Makefile.orig	2010-05-28 10:03:48.000000000 +0100
+++ src/Makefile	2010-05-28 10:10:29.000000000 +0100
@@ -7,15 +7,15 @@
 CC     = gcc
 CXX    = g++
 
-PROG        = $(NAME)
+PROG        = $(NAME)$(AB_EXEEXT)
 
 DEFAULT_CFLAGS = `sdl-config --cflags`
-LDFLAGS        = `sdl-config --libs` -lSDL_mixer -Lbulletml -lbulletml -lstdc++
+LDFLAGS        = `sdl-config --libs` -lSDL_mixer -Lbulletml -lbulletml -lstdc++ -lmikmod -lvorbisidec -static
 
 MORE_CFLAGS ?= -O3
 
 CFLAGS  = $(DEFAULT_CFLAGS) $(MORE_CFLAGS)
-CPPFLAGS  = $(DEFAULT_CFLAGS) $(MORE_CFLAGS) -I/usr/include
+CPPFLAGS  = $(DEFAULT_CFLAGS) $(MORE_CFLAGS) #-I/usr/include
 
 OBJS =	$(NAME).$(O) ship.$(O) shot.$(O) frag.$(O) bonus.$(O) \
 	foe.$(O) foecommand.$(O) barragemanager.$(O) attractmanager.$(O) \
