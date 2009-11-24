--- src/Makefile.in.orig	2009-11-24 13:15:20.000000000 -0800
+++ src/Makefile.in	2009-11-24 13:15:43.000000000 -0800
@@ -66,8 +66,8 @@
 	distdir
 ETAGS = etags
 CTAGS = ctags
-DIST_SUBDIRS = polkit polkitbackend polkitagent polkitd nullbackend \
-	programs examples
+DIST_SUBDIRS = polkit polkitbackend polkitd nullbackend \
+	#programs examples
 DISTFILES = $(DIST_COMMON) $(DIST_SOURCES) $(TEXINFOS) $(EXTRA_DIST)
 am__relativize = \
   dir0=`pwd`; \
@@ -257,8 +257,8 @@
 top_build_prefix = @top_build_prefix@
 top_builddir = @top_builddir@
 top_srcdir = @top_srcdir@
-SUBDIRS = polkit polkitbackend polkitagent polkitd nullbackend \
-	programs $(am__append_1)
+SUBDIRS = polkit polkitbackend polkitd nullbackend \
+	$(am__append_1)
 all: all-recursive
 
 .SUFFIXES:
