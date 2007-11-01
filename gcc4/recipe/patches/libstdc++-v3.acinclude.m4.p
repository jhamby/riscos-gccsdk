--- libstdc++-v3/acinclude.m4.orig	2007-10-31 18:21:02.000000000 +0000
+++ libstdc++-v3/acinclude.m4	2007-10-31 18:20:51.000000000 +0000
@@ -49,7 +49,7 @@
   # Keep these sync'd with the list in Makefile.am.  The first provides an
   # expandable list at autoconf time; the second provides an expandable list
   # (i.e., shell variable) at configure time.
-  m4_define([glibcxx_SUBDIRS],[include libmath libsupc++ src po testsuite])
+  m4_define([glibcxx_SUBDIRS],[include riscos libsupc++ po testsuite])
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
