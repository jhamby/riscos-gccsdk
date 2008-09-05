--- Makefile.orig	2007-04-09 07:14:49.000000000 +0100
+++ Makefile	2008-08-29 12:31:31.749138400 +0100
@@ -1,19 +1,18 @@
 
-CC = gcc
 
 CROSSDIR = /usr/local/cross
 STRATAGUSPATH = ../stratagus/
 
-CFLAGS = -I/usr/local/include -Wall -Wsign-compare
-LDFLAGS = -lz -lpng -lm -L/usr/local/lib
+CFLAGS = -I$(GCCSDK_INSTALL_ENV)/include -Wall -Wsign-compare
+LDFLAGS = -lz -lpng -lm -L$(GCCSDK_INSTALL_ENV)/lib -static
 
-all: wartool pudconvert
+all: wartool$(AB_EXEEXT) pudconvert$(AB_EXEEXT)
 
-wartool: wartool.o pudconvert.o
-	$(CC) -o $@ $^ $(LDFLAGS)
+wartool$(AB_EXEEXT): wartool.o pudconvert.o
+	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
 
-pudconvert: pudconvert.c
-	$(CC) -o $@ -DSTAND_ALONE pudconvert.c $(LDFLAGS)
+pudconvert$(AB_EXEEXT): pudconvert.c
+	$(CC) $(CFLAGS) -o $@ -DSTAND_ALONE pudconvert.c $(LDFLAGS)
 
 cleanobj:
 	rm -f wartool.o pudconvert.o
