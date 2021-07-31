--- libgnome/Makefile.in.orig	2013-05-29 22:53:44.721265532 +0100
+++ libgnome/Makefile.in	2013-05-29 22:53:54.209242479 +0100
@@ -311,7 +311,6 @@
 	-I$(srcdir)/..						\
 	$(WARN_CFLAGS)						\
 	$(LIBGNOME_CFLAGS)					\
-	-DG_DISABLE_DEPRECATED					\
 	-DLIBGNOME_PREFIX=\""$(prefix)"\"			\
 	-DLIBGNOME_LIBDIR=\""$(libdir)"\"			\
 	-DLIBGNOME_DATADIR=\""$(datadir)"\"			\
