--- Makefile.in.old	2004-09-13 19:57:00.000000000 +0100
+++ Makefile.in	2004-09-13 20:18:10.000000000 +0100
@@ -94,9 +94,9 @@
 	-@if [ ! -d $(includedir)  ]; then mkdir -p $(includedir); fi
 	-@if [ ! -d $(libdir)      ]; then mkdir -p $(libdir); fi
 	-@if [ ! -d $(man3dir)     ]; then mkdir -p $(man3dir); fi
-	cp zlib.h zconf.h $(includedir)
+	$(GCCSDK_INSTALL_ENV)/ro-install zlib.h zconf.h $(includedir)
 	chmod 644 $(includedir)/zlib.h $(includedir)/zconf.h
-	cp $(LIBS) $(libdir)
+	$(GCCSDK_INSTALL_ENV)/ro-install $(LIBS) $(libdir)
 	cd $(libdir); chmod 755 $(LIBS)
 	-@(cd $(libdir); $(RANLIB) libz.a || true) >/dev/null 2>&1
 	cd $(libdir); if test -f $(SHAREDLIBV); then \
@@ -105,7 +105,7 @@
 	  ln -s $(SHAREDLIBV) $(SHAREDLIBM); \
 	  (ldconfig || true)  >/dev/null 2>&1; \
 	fi
-	cp zlib.3 $(man3dir)
+	$(GCCSDK_INSTALL_ENV)/ro-install zlib.3 $(man3dir)
 	chmod 644 $(man3dir)/zlib.3
 # The ranlib in install is needed on NeXTSTEP which checks file times
 # ldconfig is for Linux
