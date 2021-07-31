--- awklib/Makefile.in.orig	2021-01-17 16:55:47.797338629 +0000
+++ awklib/Makefile.in	2021-01-17 16:56:54.472018537 +0000
@@ -109,7 +109,7 @@
 POST_UNINSTALL = :
 build_triplet = @build@
 host_triplet = @host@
-pkglibexec_PROGRAMS = pwcat$(EXEEXT) grcat$(EXEEXT)
+pkglibexec_PROGRAMS =
 subdir = awklib
 ACLOCAL_M4 = $(top_srcdir)/aclocal.m4
 am__aclocal_m4_deps = $(top_srcdir)/m4/arch.m4 \
@@ -132,12 +132,12 @@
 CONFIG_CLEAN_VPATH_FILES =
 am__installdirs = "$(DESTDIR)$(pkglibexecdir)"
 PROGRAMS = $(pkglibexec_PROGRAMS)
-nodist_grcat_OBJECTS = grcat.$(OBJEXT)
-grcat_OBJECTS = $(nodist_grcat_OBJECTS)
-grcat_LDADD = $(LDADD)
-nodist_pwcat_OBJECTS = pwcat.$(OBJEXT)
-pwcat_OBJECTS = $(nodist_pwcat_OBJECTS)
-pwcat_LDADD = $(LDADD)
+nodist_grcat_OBJECTS =
+grcat_OBJECTS =
+grcat_LDADD =
+nodist_pwcat_OBJECTS =
+pwcat_OBJECTS =
+pwcat_LDADD =
 AM_V_P = $(am__v_P_@AM_V@)
 am__v_P_ = $(am__v_P_@AM_DEFAULT_V@)
 am__v_P_0 = false
@@ -335,9 +335,9 @@
 
 # Get config.h from the build directory and custom.h from the source directory.
 AM_CPPFLAGS = -I$(top_builddir) -I$(top_srcdir)
-AUXAWK = passwd.awk group.awk
-nodist_grcat_SOURCES = grcat.c
-nodist_pwcat_SOURCES = pwcat.c
+AUXAWK =
+nodist_grcat_SOURCES =
+nodist_pwcat_SOURCES =
 CLEANFILES = $(nodist_grcat_SOURCES) $(nodist_pwcat_SOURCES)
 all: all-am
 
