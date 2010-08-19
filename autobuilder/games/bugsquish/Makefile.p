--- Makefile.orig	2010-08-19 20:56:45.000000000 +0100
+++ Makefile	2010-08-19 20:57:08.000000000 +0100
@@ -10,7 +10,7 @@
 # User-definable stuff:
 
 BIN_PREFIX=/usr/local/bin/
-DATA_PREFIX=/usr/share/bugsquish/
+DATA_PREFIX="/<Bugsquish\$$Dir>/data/"
 
 
 # Defaults for Linux:
@@ -76,7 +76,7 @@
 # Main executable:
 
 $(TARGET):	bugsquish.o
-	$(CC) $(CFLAGS) bugsquish.o -o $(TARGET) $(SDL_LIB)
+	$(CC) -static $(CFLAGS) bugsquish.o -o $(TARGET) $(SDL_LIB) -ljpeg -lpng -lz -lvorbisidec
 
 
 # Main object:
