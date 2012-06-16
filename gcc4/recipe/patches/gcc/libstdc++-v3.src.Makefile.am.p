Index: libstdc++-v3/src/Makefile.am
===================================================================
--- libstdc++-v3/src/Makefile.am	(revision 188171)
+++ libstdc++-v3/src/Makefile.am	(working copy)
@@ -24,7 +24,7 @@
 
 include $(top_srcdir)/fragment.am
 
-SUBDIRS = c++98 c++11
+SUBDIRS = c++98 c++11 . riscos
 
 # Cross compiler support.
 toolexeclib_LTLIBRARIES = libstdc++.la
