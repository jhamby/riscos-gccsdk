--- glib/Makefile.in.orig	2013-08-07 15:43:07.000000000 +0100
+++ glib/Makefile.in	2013-10-09 12:21:28.165944500 +0100
@@ -663,7 +663,7 @@
 @USE_SYSTEM_PCRE_FALSE@MAYBE_PCRE = pcre 
 SUBDIRS = libcharset $(PRINTF_SUBDIR) $(MAYBE_PCRE) update-pcre . \
 	$(am__append_1)
-DIST_SUBDIRS = libcharset gnulib pcre update-pcre tests
+DIST_SUBDIRS = libcharset gnulib pcre update-pcre
 AM_CPPFLAGS = \
 	$(glib_INCLUDES) 		\
 	$(pcre_inc) 			\
@@ -2378,7 +2378,7 @@
 @OS_UNIX_TRUE@	  && sed < "$(DESTDIR)$(bindir)/$$sf".tmp > "$(DESTDIR)$(bindir)/$$sf" \
 @OS_UNIX_TRUE@	    -e '1,24s|^ *#@PKGINSTALL_CONFIGVARS_IN24LINES@|  ${CONFIGVARS}|' \
 @OS_UNIX_TRUE@	    -e '1,1s|#! /usr/bin/env python\([0-9]\+\(\.[0-9]\+\)\?\)\?|#!${PYTHON}|' \
-@OS_UNIX_TRUE@	  || exit $$? ; \
+@OS_UNIX_TRUE@	  || true ; \
 @OS_UNIX_TRUE@	  chmod a+x $(DESTDIR)$(bindir)/$$sf ; \
 @OS_UNIX_TRUE@	  rm -f "$(DESTDIR)$(bindir)/$$sf".tmp ; \
 @OS_UNIX_TRUE@	done
