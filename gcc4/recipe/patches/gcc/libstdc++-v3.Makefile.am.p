Index: libstdc++-v3/Makefile.am
===================================================================
--- libstdc++-v3/Makefile.am	(revision 169099)
+++ libstdc++-v3/Makefile.am	(working copy)
@@ -29,7 +29,7 @@
 
 ## Keep this list sync'd with acinclude.m4:GLIBCXX_CONFIGURE.
 ## Note that python must come after src.
-SUBDIRS = include libsupc++ $(hosted_source) python
+SUBDIRS = include $(hosted_source) python
 
 ACLOCAL_AMFLAGS = -I . -I .. -I ../config
 
