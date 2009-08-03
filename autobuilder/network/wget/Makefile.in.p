--- Makefile.in.orig	2009-08-03 08:00:06.000000000 -0700
+++ Makefile.in	2009-08-03 08:00:21.000000000 -0700
@@ -83,7 +83,7 @@
 manext='$(manext)'
 
 # subdirectories in the distribution
-SUBDIRS = src doc po tests util windows
+SUBDIRS = src #doc po tests util windows
 
 # default target
 all: src/config.h Makefile $(SUBDIRS)
