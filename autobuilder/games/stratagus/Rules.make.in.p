--- Rules.make.in.orig	2021-03-28 19:52:57.626747786 +0000
+++ Rules.make.in	2021-03-28 19:54:09.677870496 +0000
@@ -19,7 +19,7 @@
 
 CPPFLAGS=@DEFS@ @CPPFLAGS@ -DUSE_ZLIB -I$(TOPDIR) -I$(TOPDIR)/src/include -I$(TOPDIR)/src/guichan/include
 CXXFLAGS=@CXXFLAGS@
-LDFLAGS=@LDFLAGS@ -lpng -lz -lm
+LDFLAGS=@LDFLAGS@ -lpng -lz -lm -llualib50 -llua50
 
 OBJDIR=@OBJDIR@
 
