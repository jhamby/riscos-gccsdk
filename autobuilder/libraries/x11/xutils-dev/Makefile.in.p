--- xorg-cf-files/Makefile.in.orig	2009-08-25 21:08:11.000000000 -0700
+++ xorg-cf-files/Makefile.in	2009-08-25 21:08:32.000000000 -0700
@@ -249,6 +249,7 @@
 	os2.cf \
 	osf1.cf \
 	pegasus.cf \
+	riscos.cf \
 	sco.cf \
 	sco5.cf \
 	sequent.cf \
--- imake/Makefile.in.orig	2010-06-05 17:33:12.167251212 +0100
+++ imake/Makefile.in	2010-06-05 17:37:39.274708645 +0100
@@ -57,7 +57,7 @@
 POST_UNINSTALL = :
 build_triplet = @build@
 host_triplet = @host@
-bin_PROGRAMS = imake$(EXEEXT) $(am__EXEEXT_1)
+bin_PROGRAMS = imake $(am__EXEEXT_1)
 subdir = .
 DIST_COMMON = README $(am__configure_deps) $(srcdir)/Makefile.am \
 	$(srcdir)/Makefile.in $(srcdir)/config.h.in \
@@ -73,7 +73,7 @@
 CONFIG_HEADER = config.h
 CONFIG_CLEAN_FILES =
 CONFIG_CLEAN_VPATH_FILES =
-@BUILD_REVPATH_TRUE@am__EXEEXT_1 = revpath$(EXEEXT)
+@BUILD_REVPATH_TRUE@am__EXEEXT_1 = revpath
 am__installdirs = "$(DESTDIR)$(bindir)" "$(DESTDIR)$(bindir)" \
 	"$(DESTDIR)$(appmandir)"
 PROGRAMS = $(bin_PROGRAMS)

