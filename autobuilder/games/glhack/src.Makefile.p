--- src/Makefile.orig	2021-03-07 11:37:17.439963694 +0000
+++ src/Makefile	2021-03-07 11:37:26.263880721 +0000
@@ -2,7 +2,7 @@
 #	SCCS Id: @(#)Makefile.src	3.4	2002/03/02
 
 CC = gcc
-LFLAGS = -L/usr/X11R6/lib
+LFLAGS = -static
 WINSRC = $(WINTTYSRC) $(WINX11SRC)
 WINOBJ = $(WINTTYOBJ) $(WINX11OBJ)
 WINTTYLIB = /usr/lib/libncurses.a
@@ -160,7 +160,7 @@
 # CFLAGS = -g -I../include
 
 CFLAGS = -O -I../include
-LFLAGS =
+LFLAGS = -static
 
 # The Qt and Be window systems are written in C++, while the rest of
 # NetHack is standard C.  If using Qt, uncomment the LINK line here to get
@@ -281,7 +281,7 @@
 WINBELIB = -lbe
 #
 # libraries for GL (SDL)
-WINGLLIB = $(shell sdl-config --libs) -L/usr/X11R6/lib -lGL -lz -lpng
+WINGLLIB = $(shell sdl-config --libs) -lz -lpng
  
 # ---->
 #
