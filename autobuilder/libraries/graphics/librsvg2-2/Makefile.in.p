--- Makefile.in.orig	2006-08-13 14:22:33.000000000 -0700
+++ Makefile.in	2006-08-13 14:25:30.000000000 -0700
@@ -43,8 +43,8 @@
 build_triplet = @build@
 host_triplet = @host@
 @ENABLE_GTK_DOC_TRUE@am__append_1 = doc
-bin_PROGRAMS = rsvg-convert$(EXEEXT) $(am__EXEEXT_1)
-noinst_PROGRAMS = test-performance$(EXEEXT)
+#bin_PROGRAMS = rsvg-convert$(EXEEXT) $(am__EXEEXT_1)
+#noinst_PROGRAMS = test-performance$(EXEEXT)
 DIST_COMMON = README $(am__configure_deps) $(librsvginc_HEADERS) \
 	$(srcdir)/Makefile.am $(srcdir)/Makefile.in \
 	$(srcdir)/config.h.in $(srcdir)/librsvg-2.0-uninstalled.pc.in \
