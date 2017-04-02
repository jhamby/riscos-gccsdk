--- glib/Makefile.am.orig	2017-02-13 15:15:29.000000000 +0000
+++ glib/Makefile.am	2017-04-01 20:02:42.472064630 +0100
@@ -34,7 +34,7 @@
 endif
 
 SUBDIRS = libcharset $(PRINTF_SUBDIR) $(MAYBE_PCRE) update-pcre . tests
-DIST_SUBDIRS = libcharset gnulib pcre update-pcre tests
+DIST_SUBDIRS = libcharset gnulib pcre update-pcre
 
 AM_CPPFLAGS = 				\
 	$(glib_INCLUDES) 		\
@@ -443,7 +443,7 @@
 	  && sed < "$(DESTDIR)$(bindir)/$$sf".tmp > "$(DESTDIR)$(bindir)/$$sf" \
 	    -e '1,24s|^ *#@PKGINSTALL_CONFIGVARS_IN24LINES@|  ${CONFIGVARS}|' \
 	    -e '1,1s|#! /usr/bin/env python.*|#!${PYTHON}|' \
-	  || exit $$? ; \
+	  || true ; \
 	  chmod a+x $(DESTDIR)$(bindir)/$$sf ; \
 	  rm -f "$(DESTDIR)$(bindir)/$$sf".tmp ; \
 	done
