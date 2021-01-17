--- awklib/Makefile.am.orig	2021-01-17 17:10:29.963878626 +0000
+++ awklib/Makefile.am	2021-01-17 17:11:00.771268171 +0000
@@ -39,10 +39,10 @@
 pkgdatadir = $(datadir)/awk
 pkglibexecdir = $(libexecdir)/awk
 
-pkglibexec_PROGRAMS = pwcat grcat
-AUXAWK = passwd.awk group.awk
-nodist_grcat_SOURCES = grcat.c
-nodist_pwcat_SOURCES = pwcat.c
+pkglibexec_PROGRAMS =
+AUXAWK =
+nodist_grcat_SOURCES =
+nodist_pwcat_SOURCES =
 
 CLEANFILES = $(nodist_grcat_SOURCES) $(nodist_pwcat_SOURCES)
 
