--- libgnome/Makefile.in.orig	2007-12-31 12:47:12.000000000 -0800
+++ libgnome/Makefile.in	2007-12-31 12:47:31.000000000 -0800
@@ -239,7 +239,7 @@
 
 libgnome_la_LDFLAGS = -version-info 36:3:4 #-rpath $(libdir)
 libgnome_la_LIBADD = $(GLIB_LIBS) -lm
-libgnome_la_LIBADD += -lesd -lpopt -laudiofile -ldb
+libgnome_la_LIBADD += -lpopt -laudiofile -ldb
 
 bin_PROGRAMS = dns-helper gnome-dump-metadata gnome-moz-remote \
 	gconfigger gnome-gen-mimedb
