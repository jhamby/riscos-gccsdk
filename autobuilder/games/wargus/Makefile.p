--- Makefile.orig	2004-07-02 11:44:34.000000000 +0100
+++ Makefile	2005-04-27 09:33:24.140625000 +0100
@@ -1,11 +1,11 @@
 
-CC = gcc
+CC = /home/riscos/cross/bin/gcc
 
 CROSSDIR = /usr/local/cross
 STRATAGUSPATH = ../stratagus/
 
-CFLAGS = -I/usr/local/include
-LDFLAGS = -lz -lpng -lm -static -L/usr/local/lib
+CFLAGS = -I/home/riscos/env/include
+LDFLAGS =  -L/home/riscos/env/lib -lz -lpng -lm -static
 
 all: cleanobj wartool$(EXE)
 
@@ -22,12 +22,11 @@
 
 cleanobj:
 	rm -f wartool.o
-
 clean:
 	rm -rf wartool wartool.exe wartool.o data.wc2 wargus-* wargus
 
 strip:
-	strip wartool
+#	strip wartool
 #	strip wartool.exe
 
 date = $(shell date +%y%m%d)
