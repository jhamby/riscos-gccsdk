--- libstdc++-v3/acinclude.m4.orig	2009-11-28 14:17:30.000000000 +0000
+++ libstdc++-v3/acinclude.m4	2009-11-28 14:17:11.000000000 +0000
@@ -49,7 +49,7 @@ AC_DEFUN([GLIBCXX_CONFIGURE], [
   # Keep these sync'd with the list in Makefile.am.  The first provides an
   # expandable list at autoconf time; the second provides an expandable list
   # (i.e., shell variable) at configure time.
-  m4_define([glibcxx_SUBDIRS],[include libmath libsupc++ src po testsuite])
+  m4_define([glibcxx_SUBDIRS],[include riscos libmath libsupc++ src po testsuite])
   SUBDIRS='glibcxx_SUBDIRS'
 
   # These need to be absolute paths, yet at the same time need to
@@ -104,7 +104,15 @@ AC_DEFUN([GLIBCXX_CONFIGURE], [
   AC_PROG_CC
   AC_PROG_CXX
   CXXFLAGS="$save_CXXFLAGS"
-  m4_rename([glibcxx_PRECIOUS],[_AC_ARG_VAR_PRECIOUS])
+  
+  # The following line is the equivalent of:
+  #
+  # m4_rename_force([glibcxx_PRECIOUS],[_AC_ARG_VAR_PRECIOUS])
+  #
+  # however, m4_rename_force is only defined in newer versions of autoconf and 
+  # not in autoconf2.59.
+  m4_ifdef([_AC_ARG_VAR_PRECIOUS], [m4_undefine([_AC_ARG_VAR_PRECIOUS])])m4_rename([glibcxx_PRECIOUS],[_AC_ARG_VAR_PRECIOUS])
+
   AC_SUBST(CFLAGS)
   AC_SUBST(CXXFLAGS)
 

