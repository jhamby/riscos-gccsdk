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
--- src/bulletml/Makefile.orig	2009-08-03 07:07:51.000000000 -0700
+++ src/bulletml/Makefile	2009-08-03 07:07:59.000000000 -0700
@@ -1,4 +1,4 @@
-CC = g++
+#CC = g++
 INCLUDES = 
 LIBS = tinyxml/tinyxml.o tinyxml/tinyxmlparser.o tinyxml/tinyxmlerror.o
 #CXXFLAGS = -g -W -Wall -ansi -pedantic
--- src/Makefile.orig	2009-08-03 07:06:56.000000000 -0700
+++ src/Makefile	2009-08-03 07:14:18.000000000 -0700
@@ -4,13 +4,13 @@
 NAME   = noiz2sa
 O      = o
 RM     = rm -f
-CC     = gcc
-CXX    = g++
+#CC     = gcc
+#CXX    = g++
 
 PROG        = $(NAME)
 
 DEFAULT_CFLAGS = `sdl-config --cflags`
-LDFLAGS        = `sdl-config --libs` -lSDL_mixer -Lbulletml -lbulletml -lstdc++ -lmikmod -lvorbisidec
+LDFLAGS        := `sdl-config --libs` -lSDL_mixer -Lbulletml -lbulletml -lstdc++ -lmikmod -lvorbisidec -static
 
 MORE_CFLAGS ?= -O3 -Ibulletml
 
