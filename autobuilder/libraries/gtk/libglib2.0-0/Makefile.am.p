--- glib/Makefile.am.orig	2014-03-24 18:49:34.000000000 +0000
+++ glib/Makefile.am	2014-05-08 17:08:34.148137594 +0100
@@ -32,7 +32,7 @@
 endif
 
 SUBDIRS = libcharset $(PRINTF_SUBDIR) $(MAYBE_PCRE) update-pcre . tests
-DIST_SUBDIRS = libcharset gnulib pcre update-pcre tests
+DIST_SUBDIRS = libcharset gnulib pcre update-pcre
 
 AM_CPPFLAGS = 				\
 	$(glib_INCLUDES) 		\
@@ -434,7 +434,7 @@
 	  && sed < "$(DESTDIR)$(bindir)/$$sf".tmp > "$(DESTDIR)$(bindir)/$$sf" \
 	    -e '1,24s|^ *#@PKGINSTALL_CONFIGVARS_IN24LINES@|  ${CONFIGVARS}|' \
 	    -e '1,1s|#! /usr/bin/env python.*|#!${PYTHON}|' \
-	  || exit $$? ; \
+	  || true ; \
 	  chmod a+x $(DESTDIR)$(bindir)/$$sf ; \
 	  rm -f "$(DESTDIR)$(bindir)/$$sf".tmp ; \
 	done
