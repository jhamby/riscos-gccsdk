--- Makefile.in.old	2004-09-24 22:15:42.000000000 +0100
+++ Makefile.in	2004-09-24 22:15:55.000000000 +0100
@@ -139,7 +139,7 @@
 sharedstatedir = @sharedstatedir@
 sysconfdir = @sysconfdir@
 target_alias = @target_alias@
-SUBDIRS = lib util doc pic
+SUBDIRS = lib #util doc pic
 WINDOWS = $(top_srcdir)/windows/GifWin.cpp \
 	  $(top_srcdir)/windows/GifWin.h \
 	  $(top_srcdir)/windows/StdAfx.h \
