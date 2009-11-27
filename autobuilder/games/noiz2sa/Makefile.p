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
 
--- src/bulletml/Makefile.orig	2009-08-03 07:07:51.000000000 -0700
+++ src/bulletml/Makefile	2009-08-03 07:07:59.000000000 -0700
@@ -1,4 +1,4 @@
-CC = g++
+#CC = g++
 INCLUDES = 
 LIBS = tinyxml/tinyxml.o tinyxml/tinyxmlparser.o tinyxml/tinyxmlerror.o
 #CXXFLAGS = -g -W -Wall -ansi -pedantic

--- src/Makefile.orig	2009-11-13 16:19:35.021464000 +0000
+++ src/Makefile	2009-11-13 16:35:33.715464000 +0000
@@ -4,13 +4,13 @@
 NAME   = noiz2sa
 O      = o
 RM     = rm -f
-CC     = gcc
-CXX    = g++
+#CC     = gcc
+#CXX    = g++
 
-PROG        = $(NAME).exe
+PROG        = $(NAME)$(AB_EXEEXT)
 
 DEFAULT_CFLAGS = `sdl-config --cflags`
-LDFLAGS        = `sdl-config --libs` -L. -lmingw32 -lmingwex -lSDL_mixer -lbulletml -mwindows -lstdc++
+LDFLAGS        = `sdl-config --libs` -lSDL_mixer -Lbulletml -lbulletml -lstdc++ -lmikmod -lvorbisidec -static
 
 MORE_CFLAGS = -O3
 
@@ -20,8 +20,7 @@
 OBJS =	$(NAME).$(O) ship.$(O) shot.$(O) frag.$(O) bonus.$(O) \
 	foe.$(O) foecommand.$(O) barragemanager.$(O) attractmanager.$(O) \
 	background.$(O) letterrender.$(O) \
-	screen.$(O) clrtbl.$(O) vector.$(O) degutil.$(O) rand.$(O) soundmanager.$(O) \
-	$(NAME)_res.$(O)
+	screen.$(O) clrtbl.$(O) vector.$(O) degutil.$(O) rand.$(O) soundmanager.$(O)
 
 $(PROG): $(OBJS) 
 	$(CC) $(CFLAGS) -o $(PROG) $(OBJS) $(LDFLAGS)
 
