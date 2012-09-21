--- Makefile.in.orig	2012-09-20 13:42:27.000000000 +0100
+++ Makefile.in	2012-09-20 13:42:44.000000000 +0100
@@ -1694,7 +1694,7 @@
 	install-htmlDATA install-includeHEADERS install-man \
 	install-nodist_includeHEADERS install-pkgconfigDATA
 	@$(NORMAL_INSTALL)
-	$(MAKE) $(AM_MAKEFLAGS) install-data-hook
+	$(MAKE) $(AM_MAKEFLAGS)
 install-dvi: install-dvi-am
 
 install-dvi-am:
@@ -1762,7 +1762,7 @@
 	distclean-tags distcleancheck distdir distuninstallcheck dvi \
 	dvi-am html html-am info info-am install install-am \
 	install-binPROGRAMS install-binSCRIPTS install-data \
-	install-data-am install-data-hook install-dist_docDATA \
+	install-data-am install-dist_docDATA \
 	install-dist_htmlDATA install-dvi install-dvi-am install-exec \
 	install-exec-am install-html install-html-am install-htmlDATA \
 	install-includeHEADERS install-info install-info-am \
