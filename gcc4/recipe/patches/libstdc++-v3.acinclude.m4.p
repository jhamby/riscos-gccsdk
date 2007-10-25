--- libstdc++-v3/acinclude.m4.orig	2007-10-19 21:56:52.000000000 +0100
+++ libstdc++-v3/acinclude.m4	2007-10-23 20:55:00.000000000 +0100
@@ -49,7 +49,7 @@
   # Keep these sync'd with the list in Makefile.am.  The first provides an
   # expandable list at autoconf time; the second provides an expandable list
   # (i.e., shell variable) at configure time.
-  m4_define([glibcxx_SUBDIRS],[include libmath libsupc++ src po testsuite])
+  m4_define([glibcxx_SUBDIRS],[include riscos po testsuite])
   SUBDIRS='glibcxx_SUBDIRS'
 
   # These need to be absolute paths, yet at the same time need to
@@ -1038,8 +1038,8 @@
           #endif
           int main()
           {
-              const char __one[] = "Äuglein Augmen";
-              const char __two[] = "Äuglein";
+              const char __one[] = "ï¿½glein Augmen";
+              const char __two[] = "ï¿½glein";
               int i;
               int j;
               __locale_t        loc;
