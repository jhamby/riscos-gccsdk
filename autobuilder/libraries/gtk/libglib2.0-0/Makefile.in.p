--- glib/Makefile.in.orig	2013-05-29 16:57:36.327229605 +0100
+++ glib/Makefile.in	2013-05-29 16:59:46.990900426 +0100
@@ -614,7 +614,7 @@
 @USE_SYSTEM_PCRE_FALSE@MAYBE_PCRE = pcre 
 SUBDIRS = libcharset $(PRINTF_SUBDIR) $(MAYBE_PCRE) update-pcre . \
 	$(am__append_1)
-DIST_SUBDIRS = libcharset gnulib pcre update-pcre tests
+DIST_SUBDIRS = libcharset gnulib pcre update-pcre
 AM_CPPFLAGS = \
 	$(glib_INCLUDES) 		\
 	$(pcre_inc) 			\
@@ -2349,7 +2349,7 @@
 @OS_UNIX_TRUE@	  && sed < "$(DESTDIR)$(bindir)/$$sf".tmp > "$(DESTDIR)$(bindir)/$$sf" \
 @OS_UNIX_TRUE@	    -e '1,24s|^ *#@PKGINSTALL_CONFIGVARS_IN24LINES@|  ${CONFIGVARS}|' \
 @OS_UNIX_TRUE@	    -e '1,1s|#! /usr/bin/env python\([0-9]\+\(\.[0-9]\+\)\?\)\?|#!${PYTHON}|' \
-@OS_UNIX_TRUE@	  || exit $$? ; \
+@OS_UNIX_TRUE@	  || true ; \
 @OS_UNIX_TRUE@	  chmod a+x $(DESTDIR)$(bindir)/$$sf ; \
 @OS_UNIX_TRUE@	  rm -f "$(DESTDIR)$(bindir)/$$sf".tmp ; \
 @OS_UNIX_TRUE@	done
