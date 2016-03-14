--- Makefile.in.orig	2016-03-13 11:54:12.922073705 +0000
+++ Makefile.in	2016-03-13 11:56:53.482073869 +0000
@@ -423,7 +422,7 @@
 
 BUILT_SOURCES = options.h settings.c settings.h
 AM_CPPFLAGS = @GLIB_CFLAGS@ @GTK_CFLAGS@ @LIBSPEC_CFLAGS@ @XML_CFLAGS@ \
-	-DFUSEDATADIR="\"${pkgdatadir}\"" @SDL_CFLAGS@ \
+	-DFUSEDATADIR="\"/<Fuse\$$Dir>/\"" @SDL_CFLAGS@ \
 	-I$(srcdir)/compat
 noinst_HEADERS = bitmap.h compat.h display.h event.h fuse.h input.h \
 	keyboard.h loader.h machine.h memory.h mempool.h menu.h \
