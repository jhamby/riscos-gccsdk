--- modules/Makefile.in.org	2006-03-13 05:56:35.000000000 +0100
+++ modules/Makefile.in	2006-04-19 13:50:59.290000000 +0200
@@ -603,7 +603,7 @@
 
 @CROSS_COMPILING_FALSE@all-local: pango.modules
 
-install-data-local: $(top_builddir)/pango/pango-querymodules
+install-data-local: $(top_builddir)/pango/pango-querymodules$(EXEEXT)
 	@if $(RUN_QUERY_MODULES_TEST) ; then 				\
 	  echo $(mkinstalldirs) $(DESTDIR)$(sysconfdir)/pango \&\& 	\
 	  $(top_builddir)/pango/pango-querymodules 			\
@@ -623,7 +623,7 @@
 uninstall-local:
 	$(RM) $(DESTDIR)$(sysconfdir)/pango/pango.modules
 
-pango.modules: $(top_builddir)/pango/pango-querymodules $(SUBDIRS)
+pango.modules: $(top_builddir)/pango/pango-querymodules$(EXEEXT) $(SUBDIRS)
 	@if find . -name '*.so' | grep 'so' > /dev/null ; then \
           echo "Writing a pango.modules file to use with tests/examples."; \
           $(top_builddir)/pango/pango-querymodules `find . -name '*.la' | grep -v '\.libs' | grep -v libpango` > $@ ;\
