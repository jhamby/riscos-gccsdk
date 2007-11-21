--- Makefile.in.orig	2007-06-19 11:26:13.000000000 +0100
+++ Makefile.in	2007-11-07 09:50:11.434922700 +0000
@@ -134,7 +134,7 @@
 	-@if [ ! -d $(DESTDIR)$(libdir)       ]; then mkdir -p $(DESTDIR)$(libdir); fi
 	-@if [ ! -d $(DESTDIR)$(man3dir)      ]; then mkdir -p $(DESTDIR)$(man3dir); fi
 	-@if [ ! -d $(DESTDIR)$(pkgconfigdir) ]; then mkdir -p $(DESTDIR)$(pkgconfigdir); fi
-	cp $(LIBS) $(DESTDIR)$(libdir)
+	$(GCCSDK_INSTALL_ENV)/ro-install $(LIBS) $(DESTDIR)$(libdir)
 	cd $(DESTDIR)$(libdir); chmod 755 $(LIBS)
 	-@(cd $(DESTDIR)$(libdir); $(RANLIB) libz.a || true) >/dev/null 2>&1
 	cd $(DESTDIR)$(libdir); if test -f $(SHAREDLIBV); then \
@@ -143,16 +143,16 @@
 	  ln -s $(SHAREDLIBV) $(SHAREDLIBM); \
 	  (ldconfig || true)  >/dev/null 2>&1; \
 	fi
-	cp zlib.3 $(DESTDIR)$(man3dir)
+	$(GCCSDK_INSTALL_ENV)/ro-install zlib.3 $(DESTDIR)$(man3dir)
 	chmod 644 $(DESTDIR)$(man3dir)/zlib.3
-	cp zlib.pc $(DESTDIR)$(pkgconfigdir)
+	$(GCCSDK_INSTALL_ENV)/ro-install zlib.pc $(DESTDIR)$(pkgconfigdir)
 	chmod 644 $(DESTDIR)$(pkgconfigdir)/zlib.pc
 # The ranlib in install is needed on NeXTSTEP which checks file times
 # ldconfig is for Linux
 
 install: install-libs
 	-@if [ ! -d $(DESTDIR)$(includedir)   ]; then mkdir -p $(DESTDIR)$(includedir); fi
-	cp zlib.h zconf.h zlibdefs.h $(DESTDIR)$(includedir)
+	$(GCCSDK_INSTALL_ENV)/ro-install zlib.h zconf.h zlibdefs.h $(DESTDIR)$(includedir)
 	chmod 644 $(DESTDIR)$(includedir)/zlib.h $(DESTDIR)$(includedir)/zconf.h $(DESTDIR)$(includedir)/zlibdefs.h
 
 uninstall:
