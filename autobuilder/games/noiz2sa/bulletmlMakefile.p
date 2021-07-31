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
