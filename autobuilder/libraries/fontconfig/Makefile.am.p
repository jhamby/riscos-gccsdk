--- Makefile.am.orig	2013-10-11 05:27:33.000000000 +0100
+++ Makefile.am	2014-05-12 15:40:31.013879014 +0100
@@ -23,7 +23,7 @@
 
 SUBDIRS=fontconfig fc-case fc-lang fc-glyphname src \
 	fc-cache fc-cat fc-list fc-match fc-pattern fc-query fc-scan \
-	fc-validate conf.d test
+	fc-validate conf.d
 if ENABLE_DOCS
 SUBDIRS += doc
 endif
