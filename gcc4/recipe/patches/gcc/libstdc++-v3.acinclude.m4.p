--- libstdc++-v3/acinclude.m4.orig	2007-01-29 11:51:01.000000000 +0100
+++ libstdc++-v3/acinclude.m4	2010-07-04 19:34:10.386900003 +0200
@@ -49,7 +49,7 @@ AC_DEFUN([GLIBCXX_CONFIGURE], [
   # Keep these sync'd with the list in Makefile.am.  The first provides an
   # expandable list at autoconf time; the second provides an expandable list
   # (i.e., shell variable) at configure time.
-  m4_define([glibcxx_SUBDIRS],[include libmath libsupc++ src po testsuite])
+  m4_define([glibcxx_SUBDIRS],[include riscos libmath libsupc++ src po testsuite])
   SUBDIRS='glibcxx_SUBDIRS'
 
   # These need to be absolute paths, yet at the same time need to
