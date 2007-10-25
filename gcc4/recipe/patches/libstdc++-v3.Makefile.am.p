--- libstdc++-v3/Makefile.am.orig	2007-10-19 21:57:38.000000000 +0100
+++ libstdc++-v3/Makefile.am	2007-10-19 20:01:09.000000000 +0100
@@ -25,10 +25,10 @@
 include $(top_srcdir)/fragment.am
 
 if GLIBCXX_HOSTED
-  hosted_source = libmath src po testsuite
+  hosted_source = po testsuite
 endif
 ## Keep this list sync'd with acinclude.m4:GLIBCXX_CONFIGURE.
-SUBDIRS = include libsupc++ $(hosted_source)
+SUBDIRS = include riscos $(hosted_source)
 
 ACLOCAL_AMFLAGS = -I . -I .. -I ../config
 
