--- Makefile.in.orig	2008-01-18 11:57:14.000000000 -0800
+++ Makefile.in	2008-01-18 11:57:41.000000000 -0800
@@ -65,7 +65,7 @@
 SDL_LIBS = @SDL_LIBS@
 VERSION = @VERSION@
 
-bin_PROGRAMS = xflame
+bin_PROGRAMS = xflame$(AB_EXEEXT)
 ACLOCAL_M4 = $(top_srcdir)/aclocal.m4
 mkinstalldirs = $(SHELL) $(top_srcdir)/mkinstalldirs
 CONFIG_CLEAN_FILES = 
@@ -158,7 +158,7 @@
 
 maintainer-clean-compile:
 
-xflame: $(xflame_OBJECTS) $(xflame_DEPENDENCIES)
+xflame$(AB_EXEEXT): $(xflame_OBJECTS) $(xflame_DEPENDENCIES)
 	@rm -f xflame
 	$(LINK) $(xflame_LDFLAGS) $(xflame_OBJECTS) $(xflame_LDADD) $(LIBS)
 
