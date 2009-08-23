--- Makefile.orig	2007-04-08 23:14:49.000000000 -0700
+++ Makefile	2009-08-22 08:58:02.000000000 -0700
@@ -1,11 +1,11 @@
 
-CC = gcc
+#CC = gcc
 
 CROSSDIR = /usr/local/cross
 STRATAGUSPATH = ../stratagus/
 
-CFLAGS = -I/usr/local/include -Wall -Wsign-compare
-LDFLAGS = -lz -lpng -lm -L/usr/local/lib
+CFLAGS = -I$(GCCSDK_INSTALL_ENV)/include -Wall -Wsign-compare
+LDFLAGS = -lz -lpng -lm -L$(GCCSDK_INSTALL_ENV)/lib -static
 
 all: wartool pudconvert
 
@@ -13,7 +13,7 @@
 	$(CC) -o $@ $^ $(LDFLAGS)
 
 pudconvert: pudconvert.c
-	$(CC) -o $@ -DSTAND_ALONE pudconvert.c $(LDFLAGS)
+	$(CC) -o $@ -DSTAND_ALONE pudconvert.c $(LDFLAGS) $(CFLAGS)
 
 cleanobj:
 	rm -f wartool.o pudconvert.o
