--- Makefile.in	2009-07-28 08:28:18.000000000 -0700
+++ Makefile.in.orig	2009-07-28 08:27:40.000000000 -0700
@@ -272,7 +272,7 @@
 top_build_prefix = @top_build_prefix@
 top_builddir = @top_builddir@
 top_srcdir = @top_srcdir@
-SUBDIRS = pango modules pango-view examples docs tools tests build
+SUBDIRS = pango modules #pango-view examples docs tools tests build
 EXTRA_DIST = \
 	autogen.sh		\
 	pango.pc.in		\
--- pango/opentype/Makefile.in.orig	2012-09-20 16:58:51.000000000 +0100
+++ pango/opentype/Makefile.in	2012-09-20 16:58:59.000000000 +0100
@@ -346,7 +346,7 @@

 libharfbuzz_la_SOURCES = $(HBSOURCES) $(HBHEADERS)
 libharfbuzz_la_CPPFLAGS = -DHAVE_GLIB $(GLIB_CFLAGS)
-libharfbuzz_la_CXXFLAGS = -fPIC
+libharfbuzz_la_CXXFLAGS =
 libharfbuzz_la_LIBADD = $(GLIB_LIBS)
 main_SOURCES = main.cc
 main_CPPFLAGS = -DHAVE_GLIB $(GLIB_CFLAGS)
