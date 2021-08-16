--- Makefile.orig	2021-08-15 18:56:46.058042296 -0700
+++ Makefile	2021-08-15 18:57:18.462251546 -0700
@@ -12,9 +12,9 @@
 
 SHELL = /bin/sh
 TAR = tar
-INSTALL = install
+INSTALL = ro-install
 
-PREFIX = /usr/local
+PREFIX = $(GCCSDK_INSTALL_ENV)
 BINDIR = $(PREFIX)/bin
 INCDIR = $(PREFIX)/include
 LIBDIR = $(PREFIX)/lib
@@ -105,19 +105,15 @@
 
 install: all install-bin install-include install-lib install-man
 install-bin: $(INSTALLABLE)
-	$(INSTALL) -d "$(DESTDIR)$(BINDIR)"
 	$(INSTALL) $^ "$(DESTDIR)$(BINDIR)"
 install-include:
-	$(INSTALL) -d "$(DESTDIR)$(INCDIR)"
 	$(INSTALL) -m 644 gif_lib.h "$(DESTDIR)$(INCDIR)"
 install-lib:
-	$(INSTALL) -d "$(DESTDIR)$(LIBDIR)"
 	$(INSTALL) -m 644 libgif.a "$(DESTDIR)$(LIBDIR)/libgif.a"
 	$(INSTALL) -m 755 libgif.so "$(DESTDIR)$(LIBDIR)/libgif.so.$(LIBVER)"
 	ln -sf libgif.so.$(LIBVER) "$(DESTDIR)$(LIBDIR)/libgif.so.$(LIBMAJOR)"
 	ln -sf libgif.so.$(LIBMAJOR) "$(DESTDIR)$(LIBDIR)/libgif.so"
 install-man:
-	$(INSTALL) -d "$(DESTDIR)$(MANDIR)/man1"
 	$(INSTALL) -m 644 $(MANUAL_PAGES) "$(DESTDIR)$(MANDIR)/man1"
 uninstall: uninstall-man uninstall-include uninstall-lib uninstall-bin
 uninstall-bin:
