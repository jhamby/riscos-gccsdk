--- Makefile.in.orig	2021-07-29 01:19:54.952798725 -0700
+++ Makefile.in	2021-07-29 01:20:21.416948017 -0700
@@ -62,10 +62,10 @@
 PACKAGE = @PACKAGE@
 SDL_CFLAGS = @SDL_CFLAGS@
 SDL_CONFIG = @SDL_CONFIG@
-SDL_LIBS = @SDL_LIBS@
+SDL_LIBS = @SDL_LIBS@ -ldl
 VERSION = @VERSION@
 
-bin_PROGRAMS = xflame
+bin_PROGRAMS = xflame$(AB_EXEEXT)
 ACLOCAL_M4 = $(top_srcdir)/aclocal.m4
 mkinstalldirs = $(SHELL) $(top_srcdir)/mkinstalldirs
 CONFIG_CLEAN_FILES = 
@@ -75,7 +75,7 @@
 DEFS = @DEFS@ -I. -I$(srcdir) 
 CPPFLAGS = @CPPFLAGS@
 LDFLAGS = @LDFLAGS@
-LIBS = @LIBS@
+LIBS = @LIBS@ -ldl
 xflame_SOURCES = xflame.c
 xflame_OBJECTS =  xflame.o
 xflame_LDADD = $(LDADD)
@@ -158,7 +158,7 @@
 
 maintainer-clean-compile:
 
-xflame: $(xflame_OBJECTS) $(xflame_DEPENDENCIES)
+xflame$(AB_EXEEXT): $(xflame_OBJECTS) $(xflame_DEPENDENCIES)
 	@rm -f xflame
 	$(LINK) $(xflame_LDFLAGS) $(xflame_OBJECTS) $(xflame_LDADD) $(LIBS)
 
