--- Makefile.in.orig	2016-02-01 21:41:46.000000000 +0000
+++ Makefile.in	2016-07-25 22:14:48.607180388 +0100
@@ -56,7 +56,7 @@
 build_triplet = @build@
 host_triplet = @host@
 @USE_VERSION_RC_FALSE@libSDL2_ttf_la_DEPENDENCIES =
-noinst_PROGRAMS = showfont$(EXEEXT) glfont$(EXEEXT)
+noinst_PROGRAMS = showfont$(EXEEXT) #glfont$(EXEEXT)
 subdir = .
 DIST_COMMON = $(srcdir)/Makefile.in $(srcdir)/Makefile.am \
 	$(top_srcdir)/configure $(am__configure_deps) \
