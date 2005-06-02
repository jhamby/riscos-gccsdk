--- vncviewer/Makefile.in.orig	2005-06-01 08:46:22.000000000 +0100
+++ vncviewer/Makefile.in	2005-06-01 08:46:29.000000000 +0100
@@ -37,7 +37,7 @@
 
 DEP_LIBS = ../rfb/librfb.a ../rdr/librdr.a
 
-EXTRA_LIBS = @ZLIB_LIB@ @X_PRE_LIBS@ @X_LIBS@ -lXmu -lXaw -lXt -lX11 -lXext @X_EXTRA_LIBS@
+EXTRA_LIBS = @ZLIB_LIB@ @X_PRE_LIBS@ @X_LIBS@ -lXmu -lXaw -lXt -lX11 -lXext @X_EXTRA_LIBS@ -lXpm
 
 
 DIR_CPPFLAGS = -I$(top_srcdir) @SOCKLEN_T_DEFINE@ @X_CFLAGS@ # X_CFLAGS are really CPPFLAGS
