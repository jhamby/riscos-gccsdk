--- GUI/xephem/Makefile.smple.old	2003-01-07 16:34:51.000000000 +0000
+++ GUI/xephem/Makefile.smple	2003-01-07 19:06:12.000000000 +0000
@@ -11,6 +11,13 @@
 LIBLNK = -L../../libastro -L../../libip
 LIBLIB = -lastro -lip
 
+CC = gcc
+CLDFLAGS = -ffast-math
+CFLAGS = $(LIBINC) $(CLDFLAGS) -O2 -Wall -I/home/riscos/env/include -I/home/riscos/env/X11R6/include
+LDFLAGS = $(LIBLNK) $(CLDFLAGS) -L/home/riscos/env/lib  -L/home/riscos/env/X11R6/lib
+XLIBS = -lXm -lXp -lXt -lXext -lX11 -lSM -lICE
+LIBS = $(XLIBS) $(LIBLIB)
+
 # linux or Mac OS X with Open Group motif installed in /usr/X11R6/{lib,include}
 # CC = cc
 # CLDFLAGS = -ffast-math
