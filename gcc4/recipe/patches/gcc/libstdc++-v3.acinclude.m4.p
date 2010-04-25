--- libstdc++-v3/acinclude.m4.orig	2010-04-17 21:44:40.502882323 +0200
+++ libstdc++-v3/acinclude.m4	2010-04-17 21:44:10.242882142 +0200
@@ -49,7 +49,7 @@ AC_DEFUN([GLIBCXX_CONFIGURE], [
   # Keep these sync'd with the list in Makefile.am.  The first provides an
   # expandable list at autoconf time; the second provides an expandable list
   # (i.e., shell variable) at configure time.
-  m4_define([glibcxx_SUBDIRS],[include libmath libsupc++ src po testsuite])
+  m4_define([glibcxx_SUBDIRS],[include riscos libmath libsupc++ src po testsuite])
   SUBDIRS='glibcxx_SUBDIRS'
 
   # These need to be absolute paths, yet at the same time need to
@@ -224,7 +224,7 @@ AC_DEFUN([GLIBCXX_CHECK_LINKER_FEATURES]
   # does some of this, but throws away the result.
   changequote(,)
   ldver=`$LD --version 2>/dev/null | head -1 | \
-         sed -e 's/GNU ld version \([0-9.][0-9.]*\).*/\1/'`
+         sed -e 's/GNU ld \(version \)\{0,1\}\(([^)]*) \)\{0,1\}\([0-9.][0-9.]*\).*/\3/'`
   changequote([,])
   glibcxx_gnu_ld_version=`echo $ldver | \
          $AWK -F. '{ if (NF<3) [$]3=0; print ([$]1*100+[$]2)*100+[$]3 }'`
