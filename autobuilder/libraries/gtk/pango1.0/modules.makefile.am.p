--- modules/Makefile.am.org	2006-06-16 01:53:00.000000000 +0200
+++ modules/Makefile.am	2006-06-16 01:54:20.000000000 +0200
@@ -26,7 +26,7 @@
 all-local: pango.modules
 endif

-install-data-local: $(top_builddir)/pango/pango-querymodules
+install-data-local: $(top_builddir)/pango/pango-querymodules$(EXEEXT)
 	@if $(RUN_QUERY_MODULES_TEST) ; then 				\
 	  echo $(mkinstalldirs) $(DESTDIR)$(sysconfdir)/pango \&\& 	\
 	  $(top_builddir)/pango/pango-querymodules 			\
@@ -46,7 +46,7 @@
 pango.modules:						\
 	$(SUBDIRS)					\
 	$(top_builddir)/config.status			\
-	$(top_builddir)/pango/pango-querymodules
+	$(top_builddir)/pango/pango-querymodules$(EXEEXT)
 	@if find . -name '*.la' | grep -v '\.libs' | grep -v libpango | grep '.' > /dev/null ; then \
           echo "Writing a pango.modules file to use with tests/examples."; \
           $(top_builddir)/pango/pango-querymodules `find . -name '*.la' | grep -v '\.libs' | grep -v libpango` > $@ ;\
