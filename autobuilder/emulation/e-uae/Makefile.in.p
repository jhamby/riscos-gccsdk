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
