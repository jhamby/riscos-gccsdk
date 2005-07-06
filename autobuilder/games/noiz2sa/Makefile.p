--- src/bulletml/Makefile.orig	2005-07-06 10:54:30.000000000 +0100
+++ src/bulletml/Makefile	2005-07-06 11:02:03.000000000 +0100
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
 
--- src/Makefile.orig	2005-07-06 10:53:25.000000000 +0100
+++ src/Makefile	2005-07-06 11:01:04.000000000 +0100
@@ -10,12 +10,12 @@
 PROG        = $(NAME)
 
 DEFAULT_CFLAGS = `sdl-config --cflags`
-LDFLAGS        = `sdl-config --libs` -lSDL_mixer -Lbulletml -lbulletml -lstdc++
+LDFLAGS        = `sdl-config --libs` -lSDL_mixer -Lbulletml -lbulletml -lstdc++ -lmikmod -lvorbisidec
 
-MORE_CFLAGS ?= -O3
+MORE_CFLAGS ?= -O3 -Ibulletml
 
 CFLAGS  = $(DEFAULT_CFLAGS) $(MORE_CFLAGS)
-CPPFLAGS  = $(DEFAULT_CFLAGS) $(MORE_CFLAGS) -I/usr/include
+CPPFLAGS  = $(DEFAULT_CFLAGS) $(MORE_CFLAGS) 
 
 OBJS =	$(NAME).$(O) ship.$(O) shot.$(O) frag.$(O) bonus.$(O) \
 	foe.$(O) foecommand.$(O) barragemanager.$(O) attractmanager.$(O) \
