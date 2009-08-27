--- src/tools/Makefile.in.orig	2005-06-10 09:50:40.000000000 +0100
+++ src/tools/Makefile.in	2005-06-10 09:50:47.000000000 +0100
@@ -3,7 +3,7 @@
 #
 
 @SET_MAKE@
-CC        = @CC@
+CC        = /usr/bin/gcc
 LDFLAGS   = @LDFLAGS@
 CFLAGS    = @CFLAGS@ -D__inline__=@ac_cv_c_inline@ -DUAE_CONSOLE
 LIBRARIES = @LIBS@
--- Makefile.in.orig	2005-06-10 10:09:04.000000000 +0100
+++ Makefile.in	2005-06-10 10:09:09.000000000 +0100
@@ -4,8 +4,8 @@
 
 all:
 	cd src && $(MAKE)
-	cp src/uae .
-	cp src/readdisk .
+	#cp src/uae .
+	#cp src/readdisk .
 
 clean:
 	cd src && $(MAKE) clean
