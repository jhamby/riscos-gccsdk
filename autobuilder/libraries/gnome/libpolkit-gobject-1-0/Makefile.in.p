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
--- docs/man/Makefile.in.orig	2010-06-05 11:36:41.707204489 +0100
+++ docs/man/Makefile.in	2010-06-05 11:36:53.107196736 +0100
@@ -545,7 +545,7 @@
 
 
 @MAN_PAGES_ENABLED_TRUE@%.8 %.1 : %.xml
-@MAN_PAGES_ENABLED_TRUE@	$(XSLTPROC) -nonet --stringparam man.base.url.for.relative.links $(datadir)/gtk-doc/html/polkit-1/ --xinclude http://docbook.sourceforge.net/release/xsl/current/manpages/docbook.xsl $<
+@MAN_PAGES_ENABLED_TRUE@	$(XSLTPROC) --stringparam man.base.url.for.relative.links $(datadir)/gtk-doc/html/polkit-1/ --xinclude http://docbook.sourceforge.net/release/xsl/current/manpages/docbook.xsl $<
 
 clean-local:
 	rm -f *~ *.1 *.8

