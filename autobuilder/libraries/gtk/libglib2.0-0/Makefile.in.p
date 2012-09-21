--- glib/Makefile.in.orig	2012-05-14 23:09:30.000000000 +0100
+++ glib/Makefile.in	2012-09-20 15:15:15.000000000 +0100
@@ -555,8 +555,8 @@
 @HAVE_GOOD_PRINTF_FALSE@PRINTF_SUBDIR = gnulib
 @HAVE_GOOD_PRINTF_FALSE@printf_la = gnulib/libgnulib.la
 @USE_SYSTEM_PCRE_FALSE@MAYBE_PCRE = pcre
-SUBDIRS = libcharset $(PRINTF_SUBDIR) $(MAYBE_PCRE) update-pcre . tests
-DIST_SUBDIRS = libcharset gnulib pcre update-pcre tests
+SUBDIRS = libcharset $(PRINTF_SUBDIR) $(MAYBE_PCRE) update-pcre .
+DIST_SUBDIRS = libcharset gnulib pcre update-pcre
 AM_CPPFLAGS = \
 	$(glib_INCLUDES) 		\
 	$(pcre_inc) 			\
@@ -1718,7 +1718,7 @@
 @OS_UNIX_TRUE@	  && sed < "$(DESTDIR)$(bindir)/$$sf".tmp > "$(DESTDIR)$(bindir)/$$sf" \
 @OS_UNIX_TRUE@	    -e '1,24s|^ *#@PKGINSTALL_CONFIGVARS_IN24LINES@|  ${CONFIGVARS}|' \
 @OS_UNIX_TRUE@	    -e '1,1s|#! /usr/bin/env python\([0-9]\+\(\.[0-9]\+\)\?\)\?|#!${PYTHON}|' \
-@OS_UNIX_TRUE@	  || exit $$? ; \
+@OS_UNIX_TRUE@	  || true ; \
 @OS_UNIX_TRUE@	  chmod a+x $(DESTDIR)$(bindir)/$$sf ; \
 @OS_UNIX_TRUE@	  rm -f "$(DESTDIR)$(bindir)/$$sf".tmp ; \
 @OS_UNIX_TRUE@	done

@@ -1800,7 +1800,7 @@
 @OS_UNIX_TRUE@	  && sed < "$(DESTDIR)$(bindir)/$$sf".tmp > "$(DESTDIR)$(bindir)/$$sf" \
 @OS_UNIX_TRUE@	    -e '1,24s|^ *#@PKGINSTALL_CONFIGVARS_IN24LINES@|  ${CONFIGVARS}|' \
 @OS_UNIX_TRUE@	    -e '1,1s|#! /usr/bin/env python\([0-9]\+\(\.[0-9]\+\)\?\)\?|#!${PYTHON}|' \
-@OS_UNIX_TRUE@	  || exit $$? ; \
+@OS_UNIX_TRUE@	  || true ; \
 @OS_UNIX_TRUE@	  chmod a+x $(DESTDIR)$(bindir)/$$sf ; \
 @OS_UNIX_TRUE@	  rm -f "$(DESTDIR)$(bindir)/$$sf".tmp ; \
 @OS_UNIX_TRUE@	done
