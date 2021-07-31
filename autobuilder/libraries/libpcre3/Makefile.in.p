--- Makefile.in.orig	2015-05-06 16:24:38.821518272 +0100
+++ Makefile.in	2015-05-06 16:24:38.853518273 +0100
@@ -3027,7 +3027,7 @@
 	install-htmlDATA install-includeHEADERS install-man \
 	install-nodist_includeHEADERS install-pkgconfigDATA
 	@$(NORMAL_INSTALL)
-	$(MAKE) $(AM_MAKEFLAGS) install-data-hook
+	$(MAKE) $(AM_MAKEFLAGS)
 install-dvi: install-dvi-am
 
 install-dvi-am:
