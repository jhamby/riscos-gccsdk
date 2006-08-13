--- tools/convertrgb/Makefile.in.old	2003-01-15 12:26:40.000000000 +0000
+++ tools/convertrgb/Makefile.in	2003-01-15 12:32:08.000000000 +0000
@@ -71,7 +71,7 @@
 BUILD_INCLUDED_LIBINTL = @BUILD_INCLUDED_LIBINTL@
 CATALOGS = @CATALOGS@
 CATOBJEXT = @CATOBJEXT@
-CC = @CC@
+CC = /usr/bin/cc
 DATADIRNAME = @DATADIRNAME@
 DLLTOOL = @DLLTOOL@
 ECHO = @ECHO@
--- libart_lgpl/Makefile.in.old	2003-05-02 20:20:52.000000000 +0100
+++ libart_lgpl/Makefile.in	2003-05-02 19:24:18.000000000 +0100
@@ -45,7 +45,7 @@
 AUTOMAKE = @AUTOMAKE@
 AUTOHEADER = @AUTOHEADER@
 
-INSTALL = @INSTALL@
+INSTALL = $(GCCSDK_INSTALL_ENV)/ro-install
 INSTALL_PROGRAM = @INSTALL_PROGRAM@ $(AM_INSTALL_PROGRAM_FLAGS)
 INSTALL_DATA = @INSTALL_DATA@
 INSTALL_SCRIPT = @INSTALL_SCRIPT@
--- libart_lgpl/doc/Makefile.in.old	2002-08-15 23:25:26.000000000 +0100
+++ libart_lgpl/doc/Makefile.in	2003-05-02 19:27:54.000000000 +0100
@@ -47,7 +47,7 @@
 AUTOMAKE = @AUTOMAKE@
 AUTOHEADER = @AUTOHEADER@
 
-INSTALL = @INSTALL@
+INSTALL = $(GCCSDK_INSTALL_ENV)/ro-install
 INSTALL_PROGRAM = @INSTALL_PROGRAM@ $(AM_INSTALL_PROGRAM_FLAGS)
 INSTALL_DATA = @INSTALL_DATA@
 INSTALL_SCRIPT = @INSTALL_SCRIPT@
--- gnome-data/Makefile.in.orig	2006-08-13 09:01:19.000000000 -0700
+++ gnome-data/Makefile.in	2006-08-13 09:01:36.000000000 -0700
@@ -388,7 +388,7 @@
 install-data-local:
 	$(mkinstalldirs) $(DESTDIR)$(scoredir)
 	-chgrp $(GAMES_GROUP) $(DESTDIR)$(scoredir) && chmod g=rwXs $(DESTDIR)$(scoredir)
-	$(top_builddir)/libgnome/gnome-gen-mimedb $(DESTDIR)$(sysconfdir)/mime-magic
+	gnome-gen-mimedb $(DESTDIR)$(sysconfdir)/mime-magic
 
 # Tell versions [3.59,3.63) of GNU make to not export all variables.
 # Otherwise a system limit (for SysV at least) may be exceeded.
--- libgnorba/Makefile.in.orig	2006-08-13 10:02:27.000000000 -0700
+++ libgnorba/Makefile.in	2006-08-13 10:01:42.000000000 -0700
@@ -1188,7 +1188,7 @@
 
 install-data-hook:
 	$(RM) -f $(DESTDIR)$(bindir)/new-object
-	$(LN_S) goad-browser $(DESTDIR)$(bindir)/new-object
+	$(LN_S) -f goad-browser $(DESTDIR)$(bindir)/new-object
 
 clean-local:
 	$(RM) -f $(BUILT_SOURCES)
--- libgnome/Makefile.in.orig	2006-08-13 11:07:51.000000000 -0700
+++ libgnome/Makefile.in	2006-08-13 11:08:21.000000000 -0700
@@ -239,7 +239,7 @@
 
 libgnome_la_LDFLAGS = -version-info 36:3:4 #-rpath $(libdir)
 libgnome_la_LIBADD = $(GLIB_LIBS) -lm
-libgnome_la_LIBADD += -lesd -lpopt -laudiofile -ldb3
+libgnome_la_LIBADD += -lpopt -laudiofile -ldb3
 
 bin_PROGRAMS = dns-helper gnome-dump-metadata gnome-moz-remote \
 	gconfigger gnome-gen-mimedb
