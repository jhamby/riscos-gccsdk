--- sys/unix/Makefile.utl	2003-01-31 21:53:44.000000000 +0000
+++ sys/unix/Makefile.utl.new	2003-01-31 21:52:59.000000000 +0000
@@ -72,8 +72,8 @@
 
 # flags for Linux
 #   compile normally
-CFLAGS = -O2 -fomit-frame-pointer -I../include
-LFLAGS = -L/usr/X11R6/lib
+CFLAGS = -O2 -fomit-frame-pointer -I../include -I/home/riscos/env/X11R6/include
+LFLAGS = -L/home/riscos/env/X11R6/lib
 #   OR compile backwards compatible a.out format
 # CFLAGS = -O2 -b i486-linuxaout -fomit-frame-pointer -I../include
 # LFLAGS = -b i486-linuxaout -L/usr/X11R6/lib
