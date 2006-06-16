--- Makefile.am.org	2006-06-16 23:54:26.000000000 +0200
+++ Makefile.am	2006-06-16 23:54:43.000000000 +0200
@@ -1,8 +1,8 @@
 ## Process this file with automake to produce Makefile.in
 
-SUBDIRS = include . doc example xstc @PYTHON_SUBDIR@
+SUBDIRS = include . doc xstc
 
-DIST_SUBDIRS = include . doc example python xstc
+DIST_SUBDIRS = include . doc xstc
 
 INCLUDES = -I$(top_builddir)/include -I@srcdir@/include @THREAD_CFLAGS@ @Z_CFLAGS@ 
 
