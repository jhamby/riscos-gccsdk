--- tests/handles/Makefile.in.old	2003-01-08 15:32:59.000000000 +0000
+++ tests/handles/Makefile.in	2003-01-08 15:33:16.000000000 +0000
@@ -103,7 +103,7 @@
 
 DEFS = @DEFS@ -I. -I$(srcdir) -I../../sigc++/config
 CPPFLAGS = @CPPFLAGS@
-LDFLAGS = @LDFLAGS@
+LDFLAGS = @LDFLAGS@ -liostream
 LIBS = @LIBS@
 ctor_OBJECTS =  ctor.o
 ctor_LDADD = $(LDADD)
--- Makefile.in.old	2003-01-08 15:48:14.000000000 +0000
+++ Makefile.in	2003-01-08 15:48:22.000000000 +0000
@@ -81,7 +81,7 @@
 THREAD_LIB = @THREAD_LIB@
 VERSION = @VERSION@
 
-SUBDIRS = sigc++ tests 
+SUBDIRS = sigc++ #tests 
 DIST_SUBDIRS = sigc++ examples tests scripts
 
 bin_SCRIPTS = sigc-config
