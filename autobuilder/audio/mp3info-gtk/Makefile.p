--- Makefile	2006-08-20 12:36:17.510176376 +1200
+++ Makefile.001	2006-08-20 12:37:10.281153968 +1200
@@ -43,9 +43,9 @@
 INSTALL = /usr/bin/install -c
 STRIP = strip
 
-LIBS = -lncurses
+LIBS = -L/home/riscos/env/lib -lncurses
 CC = gcc
-CFLAGS = -g -O2 -Wall
+CFLAGS = -I/home/riscos/env/include -I/home/riscos/env/include/ncurses -O2 -Wall
 
 all: mp3info gmp3info doc
 
