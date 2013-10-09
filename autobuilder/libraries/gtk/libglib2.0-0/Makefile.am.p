--- glib/Makefile.am.orig	2013-08-07 15:34:32.000000000 +0100
+++ glib/Makefile.am	2013-10-09 12:21:28.185944500 +0100
@@ -36,7 +36,7 @@
 endif
 
 SUBDIRS = libcharset $(PRINTF_SUBDIR) $(MAYBE_PCRE) update-pcre .
-DIST_SUBDIRS = libcharset gnulib pcre update-pcre tests
+DIST_SUBDIRS = libcharset gnulib pcre update-pcre
 if BUILD_MODULAR_TESTS
 SUBDIRS += tests 
 endif
@@ -431,7 +431,7 @@
 	  && sed < "$(DESTDIR)$(bindir)/$$sf".tmp > "$(DESTDIR)$(bindir)/$$sf" \
 	    -e '1,24s|^ *#@PKGINSTALL_CONFIGVARS_IN24LINES@|  ${CONFIGVARS}|' \
 	    -e '1,1s|#! /usr/bin/env python\([0-9]\+\(\.[0-9]\+\)\?\)\?|#!${PYTHON}|' \
-	  || exit $$? ; \
+	  || true ; \
 	  chmod a+x $(DESTDIR)$(bindir)/$$sf ; \
 	  rm -f "$(DESTDIR)$(bindir)/$$sf".tmp ; \
 	done
