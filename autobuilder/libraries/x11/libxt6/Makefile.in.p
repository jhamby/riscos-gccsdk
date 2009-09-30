--- util/Makefile.in	2009-07-03 05:25:58.000000000 +1200
+++ util/Makefile.in	2009-10-01 12:50:24.000000000 +1300
@@ -33,8 +33,8 @@
 POST_UNINSTALL = :
 build_triplet = @build@
 host_triplet = @host@
-@INSTALL_MAKESTRS_TRUE@bin_PROGRAMS = makestrs$(EXEEXT)
-@INSTALL_MAKESTRS_FALSE@noinst_PROGRAMS = makestrs$(EXEEXT)
+@INSTALL_MAKESTRS_TRUE@bin_PROGRAMS = makestrs
+@INSTALL_MAKESTRS_FALSE@noinst_PROGRAMS = makestrs
 subdir = util
 DIST_COMMON = $(srcdir)/Makefile.am $(srcdir)/Makefile.in
 ACLOCAL_M4 = $(top_srcdir)/aclocal.m4
