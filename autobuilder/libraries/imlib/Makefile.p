--- utils/Makefile.in.orig	2007-12-29 10:47:14.000000000 -0800
+++ utils/Makefile.in	2007-12-29 10:47:22.000000000 -0800
@@ -36,7 +36,7 @@
 POST_UNINSTALL = :
 build_triplet = @build@
 host_triplet = @host@
-bin_PROGRAMS = imlib_config$(EXEEXT)
+#bin_PROGRAMS = imlib_config$(EXEEXT)
 subdir = utils
 DIST_COMMON = $(srcdir)/Makefile.am $(srcdir)/Makefile.in install-sh \
 	mkinstalldirs
