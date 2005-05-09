--- Makefile.orig	2005-05-09 17:19:18.000000000 +0100
+++ Makefile	2005-05-09 17:20:56.000000000 +0100
@@ -1,9 +1,9 @@
 # Executables
-CC     = gcc
+CC     = gcc -L/home/riscos/env/lib
 INSTALL= /usr/bin/install
 
 # Flags
-CFLAGS = -I. -O3 -g -Wall
+CFLAGS = -I. -O3 -g -Wall -I/home/riscos/env/include
 
 SRC = dcraw.c parse.c fixdates.c
 OBJ = dcraw.o parse.o fixdates.o
@@ -12,13 +12,13 @@
 default: dcraw dcparse dcfixdates
 
 dcraw: dcraw.o
-	$(CC) -s -o $@ dcraw.o -lm -ljpeg
+	$(CC) -o $@ dcraw.o -lm -ljpeg
 
 dcparse: parse.o
-	$(CC) -s -o $@ parse.o
+	$(CC) -o $@ parse.o
 
 dcfixdates: fixdates.o
-	$(CC) -s -o $@ fixdates.o
+	$(CC) -o $@ fixdates.o
 
 clean:
 	rm -f core *.o dcraw dcparse dcfixdates
