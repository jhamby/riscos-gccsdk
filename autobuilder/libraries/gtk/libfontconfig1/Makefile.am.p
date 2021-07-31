--- Makefile.am.orig	2014-05-18 17:00:47.191567791 +0100
+++ Makefile.am	2014-05-18 17:00:58.263338084 +0100
@@ -23,7 +23,7 @@
 
 SUBDIRS=fontconfig fc-case fc-lang fc-glyphname src \
 	fc-cache fc-cat fc-list fc-match fc-pattern fc-query fc-scan \
-	fc-validate conf.d test
+	fc-validate conf.d
 if ENABLE_DOCS
 SUBDIRS += doc
 endif
