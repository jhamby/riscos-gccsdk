--- sys/unix/Makefile.src.old	2003-01-31 21:17:05.000000000 +0000
+++ sys/unix/Makefile.src	2003-02-01 16:25:28.000000000 +0000
@@ -117,8 +117,8 @@
 
 # flags for Linux
 #   compile normally
-CFLAGS = -O2 -fomit-frame-pointer -I../include
-LFLAGS = -L/usr/X11R6/lib
+CFLAGS = -O2 -fomit-frame-pointer -I../include -I/home/riscos/env/include -I/home/riscos/env/X11R6/include
+LFLAGS = -L/home/riscos/env/lib -L/home/riscos/env/X11R6/lib
 #   OR compile backwards compatible a.out format
 # CFLAGS = -O2 -b i486-linuxaout -fomit-frame-pointer -I../include
 # LFLAGS = -b i486-linuxaout -L/usr/X11R6/lib
@@ -442,11 +442,11 @@
 # below in the 'make depend' output.
 monst.o:
 	$(CC) $(CFLAGS) -c monst.c
-	@rm -f $(MAKEDEFS)
+	#@rm -f $(MAKEDEFS)
 
 objects.o:
 	$(CC) $(CFLAGS) -c objects.c
-	@rm -f $(MAKEDEFS)
+	#@rm -f $(MAKEDEFS)
 
 # Qt windowport meta-object-compiler output
 qt_kde0.moc: ../include/qt_kde0.h
