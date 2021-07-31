--- Makefile.old	2003-04-25 08:34:23.000000000 +0100
+++ Makefile	2003-04-25 07:39:08.000000000 +0100
@@ -113,6 +113,10 @@
 generic:
 	make -e $(ALL) $(MFLAGS) XFLAGS='-DGENERIC' STATIC=
 
+# RISC OS by Peter Naulls
+riscos:
+	make -e $(ALL) $(MFLAGS) XFLAGS='-DRISOS -O2 -DDEBIAN_VERSION=\"1.10\"' STATIC=-static
+
 # Still at large: dgux dynix ???
 
 ### RANDOM
--- Makefile.orig	2008-01-26 17:20:54.000000000 -0800
+++ Makefile	2008-01-26 17:21:08.000000000 -0800
@@ -14,7 +14,7 @@
 XLIBS =		# xtra libs if necessary?
 # -Bstatic for sunos,  -static for gcc, etc.  You want this, trust me.
 STATIC =
-CC = gcc $(CFLAGS)
+CC := $(CC) $(CFLAGS)
 LD = $(CC)	# linker; defaults to unstripped executables
 o = o		# object extension
 
