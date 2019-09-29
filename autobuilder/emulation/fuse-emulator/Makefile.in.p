--- Makefile.in.orig	2019-09-29 23:27:49.480628389 +0100
+++ Makefile.in	2019-09-29 23:29:57.864631055 +0100
@@ -864,7 +864,7 @@
               $(GTK_CFLAGS) \
               $(LIBSPEC_CFLAGS) \
               $(XML_CFLAGS) \
-              -DFUSEDATADIR="\"${pkgdatadir}\"" \
+              -DFUSEDATADIR="\"/<Fuse\$Dir>/\"" \
               $(SDL_CFLAGS) \
               $(PNG_CFLAGS)
 
