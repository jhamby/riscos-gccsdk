--- Makefile.orig	2009-07-29 18:50:03.000000000 -0700
+++ Makefile	2009-07-29 18:53:47.000000000 -0700
@@ -43,9 +43,8 @@
 INSTALL = /usr/bin/install -c
 STRIP = strip
 
-LIBS = -lncurses
-CC = gcc
-CFLAGS = -g -O2 -Wall
+LIBS = -L$(GCCSDK_INSTALL_ENV)/lib -lncurses -Wl,-rpath-link,$(GCCSDK_INSTALL_ENV)/lib -ldl
+CFLAGS = -I$(GCCSDK_INSTALL_ENV)/include -I$(GCCSDK_INSTALL_ENV)/include/ncurses -O2 -Wall
 
 all: mp3info gmp3info doc
 
@@ -55,7 +54,7 @@
 	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)
 
 gmp3info: $(XSRC) $(XOBJ) 
-	$(CC) $(XSRC) $(CFLAGS) -o $@ $(XOBJ) `pkg-config --cflags --libs gtk+-2.0`
+	$(CC) $(XSRC) $(CFLAGS) $(LIBS) -o $@ $(XOBJ) `pkg-config --cflags --libs gtk+-2.0`
 
 mp3info.txt: mp3info.1
 	groff -t -e -mandoc -Tascii mp3info.1 | col -bx > mp3info.txt
