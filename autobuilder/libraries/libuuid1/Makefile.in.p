--- shlibs/uuid/src/Makefile.in.orig	2011-12-31 09:34:20.000000000 +0000
+++ shlibs/uuid/src/Makefile.in	2011-12-31 09:34:41.000000000 +0000
@@ -564,7 +564,7 @@
 	done
 check-am: all-am
 check: check-am
-all-am: Makefile $(LTLIBRARIES) $(PROGRAMS) $(DATA) $(HEADERS)
+all-am: Makefile $(LTLIBRARIES) $(DATA) $(HEADERS)
 installdirs:
 	for dir in "$(DESTDIR)$(usrlib_execdir)" "$(DESTDIR)$(uuidincdir)"; do \
 	  test -z "$$dir" || $(MKDIR_P) "$$dir"; \
