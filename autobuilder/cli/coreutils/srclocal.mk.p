--- src/local.mk.orig	2014-07-18 19:29:24.000000000 +0100
+++ src/local.mk	2015-05-02 20:02:06.317553118 +0100
@@ -32,7 +32,7 @@
 
 # The user can tweak these lists at configure time.
 bin_PROGRAMS = @bin_PROGRAMS@
-pkglibexec_PROGRAMS = @pkglibexec_PROGRAMS@
+pkglibexec_PROGRAMS =
 
 # Needed by the testsuite.
 noinst_PROGRAMS =		\
@@ -392,15 +392,6 @@
 
 src_ginstall_CPPFLAGS = -DENABLE_MATCHPATHCON=1 $(AM_CPPFLAGS)
 
-# Ensure we don't link against libcoreutils.a as that lib is
-# not compiled with -fPIC which causes issues on 64 bit at least
-src_libstdbuf_so_LDADD = $(LIBINTL)
-
-# Note libstdbuf is only compiled if GCC is available
-# (as per the check in configure.ac), so these flags should be available.
-# libtool is probably required to relax this dependency.
-src_libstdbuf_so_LDFLAGS = -shared
-src_libstdbuf_so_CFLAGS = -fPIC $(AM_CFLAGS)
 
 BUILT_SOURCES += src/coreutils.h
 if SINGLE_BINARY
@@ -599,7 +590,6 @@
 	 | sed -e 's,$(EXEEXT)$$,,' \
 	       -e 's,^src/,,' \
 	       -e 's/^ginstall$$/install/' \
-	 | sed /libstdbuf/d \
 	 | $(ASSORT) -u > $(pm) && \
 	sed -n '/^The programs .* are:/,/^[a-zA-Z]/p' $(top_srcdir)/README \
 	  | sed -n '/^   */s///p' | tr -s ' ' '\n' > $(pr)


