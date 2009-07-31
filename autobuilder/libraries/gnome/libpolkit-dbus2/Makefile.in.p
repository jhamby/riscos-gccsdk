--- Makefile.in.orig	2009-07-31 12:27:02.000000000 -0700
+++ Makefile.in	2009-07-31 12:27:07.000000000 -0700
@@ -251,7 +251,7 @@
 top_build_prefix = @top_build_prefix@
 top_builddir = @top_builddir@
 top_srcdir = @top_srcdir@
-SUBDIRS = data src polkitd doc tools policy po test
+SUBDIRS = data src polkitd doc #tools policy po test
 EXTRA_DIST = \
 	HACKING 		\
 	mkinstalldirs 		\
--- src/polkit-dbus/Makefile.in.orig	2009-07-31 12:24:45.000000000 -0700
+++ src/polkit-dbus/Makefile.in	2009-07-31 12:24:52.000000000 -0700
@@ -34,7 +34,7 @@
 POST_UNINSTALL = :
 build_triplet = @build@
 host_triplet = @host@
-libexec_PROGRAMS = polkit-resolve-exe-helper$(EXEEXT) $(am__EXEEXT_2)
+libexec_PROGRAMS = #polkit-resolve-exe-helper$(EXEEXT) $(am__EXEEXT_2)
 @POLKIT_AUTHDB_DEFAULT_TRUE@am__append_1 = polkit-read-auth-helper polkit-set-default-helper
 @KIT_BUILD_TESTS_TRUE@TESTS = polkit-dbus-test$(EXEEXT)
 @KIT_BUILD_TESTS_TRUE@check_PROGRAMS = $(am__EXEEXT_1)
