--- glib/Makefile.am.orig	2012-09-21 19:26:17.436666000 +0100
+++ glib/Makefile.am	2012-09-21 19:31:01.781803000 +0100
@@ -35,9 +35,9 @@
 MAYBE_PCRE = pcre 
 endif
 
-SUBDIRS = libcharset $(PRINTF_SUBDIR) $(MAYBE_PCRE) update-pcre . tests
+SUBDIRS = libcharset $(PRINTF_SUBDIR) $(MAYBE_PCRE) update-pcre .
 
-DIST_SUBDIRS = libcharset gnulib pcre update-pcre tests
+DIST_SUBDIRS = libcharset gnulib pcre update-pcre
 
 AM_CPPFLAGS = 				\
 	$(glib_INCLUDES) 		\
@@ -451,7 +451,7 @@
 	  && sed < "$(DESTDIR)$(bindir)/$$sf".tmp > "$(DESTDIR)$(bindir)/$$sf" \
 	    -e '1,24s|^ *#@PKGINSTALL_CONFIGVARS_IN24LINES@|  ${CONFIGVARS}|' \
 	    -e '1,1s|#! /usr/bin/env python\([0-9]\+\(\.[0-9]\+\)\?\)\?|#!${PYTHON}|' \
-	  || exit $$? ; \
+	  || true ; \
 	  chmod a+x $(DESTDIR)$(bindir)/$$sf ; \
 	  rm -f "$(DESTDIR)$(bindir)/$$sf".tmp ; \
 	done
