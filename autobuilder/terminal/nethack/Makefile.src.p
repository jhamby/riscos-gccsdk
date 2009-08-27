--- sys/unix/Makefile.src.orig	2009-08-27 10:29:22.000000000 -0700
+++ sys/unix/Makefile.src	2009-08-27 10:32:50.000000000 -0700
@@ -124,8 +124,10 @@
 
 # flags for Linux
 #   compile normally
-# CFLAGS = -O2 -fomit-frame-pointer -I../include
-LFLAGS = -L/usr/X11R6/lib
+CFLAGS = -O2 -fomit-frame-pointer -I../include -I$(GCCSDK_INSTALL_ENV)/include
+#LFLAGS = -L/usr/X11R6/lib
+LDFLAGS = -L$(GCCSDK_INSTALL_ENV)/lib
+
 #   OR compile backwards compatible a.out format
 # CFLAGS = -O2 -b i486-linuxaout -fomit-frame-pointer -I../include
 # LFLAGS = -b i486-linuxaout -L/usr/X11R6/lib
@@ -480,11 +482,11 @@
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
