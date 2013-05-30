--- Makefile.in.orig	2012-09-20 17:37:57.000000000 +0100
+++ Makefile.in	2012-09-20 17:38:43.000000000 +0100
@@ -202,7 +202,7 @@
 	distdir dist dist-all distcheck
 ETAGS = etags
 CTAGS = ctags
-DIST_SUBDIRS = . gdk-pixbuf-loader tests tools doc gtk-engine
+DIST_SUBDIRS = . tools doc
 DISTFILES = $(DIST_COMMON) $(DIST_SOURCES) $(TEXINFOS) $(EXTRA_DIST)
 distdir = $(PACKAGE)-$(VERSION)
 top_distdir = $(distdir)
@@ -409,7 +409,7 @@
 top_build_prefix = @top_build_prefix@
 top_builddir = @top_builddir@
 top_srcdir = @top_srcdir@
-SUBDIRS = . gdk-pixbuf-loader tests tools doc $(am__append_1)
+SUBDIRS = . tests tools doc
 NULL = 
 ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}
 lib_LTLIBRARIES = librsvg-@RSVG_API_MAJOR_VERSION@.la
