--- glib/Makefile.in.orig	2014-03-24 18:54:29.000000000 +0000
+++ glib/Makefile.in	2014-05-08 17:08:34.064137593 +0100
@@ -969,7 +969,7 @@
 @HAVE_GOOD_PRINTF_FALSE@printf_la = gnulib/libgnulib.la
 @USE_SYSTEM_PCRE_FALSE@MAYBE_PCRE = pcre 
 SUBDIRS = libcharset $(PRINTF_SUBDIR) $(MAYBE_PCRE) update-pcre . tests
-DIST_SUBDIRS = libcharset gnulib pcre update-pcre tests
+DIST_SUBDIRS = libcharset gnulib pcre update-pcre
 AM_CPPFLAGS = \
 	$(glib_INCLUDES) 		\
 	$(pcre_inc) 			\
@@ -3027,7 +3027,7 @@
 @OS_UNIX_TRUE@	  && sed < "$(DESTDIR)$(bindir)/$$sf".tmp > "$(DESTDIR)$(bindir)/$$sf" \
 @OS_UNIX_TRUE@	    -e '1,24s|^ *#@PKGINSTALL_CONFIGVARS_IN24LINES@|  ${CONFIGVARS}|' \
 @OS_UNIX_TRUE@	    -e '1,1s|#! /usr/bin/env python.*|#!${PYTHON}|' \
-@OS_UNIX_TRUE@	  || exit $$? ; \
+@OS_UNIX_TRUE@	  || true ; \
 @OS_UNIX_TRUE@	  chmod a+x $(DESTDIR)$(bindir)/$$sf ; \
 @OS_UNIX_TRUE@	  rm -f "$(DESTDIR)$(bindir)/$$sf".tmp ; \
 @OS_UNIX_TRUE@	done
