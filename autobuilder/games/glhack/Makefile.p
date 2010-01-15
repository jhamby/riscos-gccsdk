--- src/Makefile.orig	2009-08-25 08:26:23.000000000 -0700
+++ src/Makefile	2009-08-25 08:31:01.000000000 -0700
@@ -159,7 +159,7 @@
 # flags for debugging:
 # CFLAGS = -g -I../include
 
-CFLAGS = -O -I../include
+CFLAGS = -O2 -I../include
 LFLAGS = 
 
 # The Qt and Be window systems are written in C++, while the rest of
@@ -281,7 +281,7 @@
 WINBELIB = -lbe
 #
 # libraries for GL (SDL)
-WINGLLIB = $(shell sdl-config --libs) -L/usr/X11R6/lib -lGL -lz -lpng
+WINGLLIB = $(shell sdl-config --libs) -lz -lpng -static
  
 # ---->
 #
@@ -485,10 +485,12 @@
 # below in the 'make depend' output.
 monst.o:
 	$(CC) $(CFLAGS) -c monst.c
+	/usr/bin/gcc $(CFLAGS) -c -o monstnative.o monst.c
 	@rm -f $(MAKEDEFS)
 
 objects.o:
 	$(CC) $(CFLAGS) -c objects.c
+	/usr/bin/gcc $(CFLAGS) -c -o objectsnative.o objects.c
 	@rm -f $(MAKEDEFS)
 
 # Qt windowport meta-object-compiler output
