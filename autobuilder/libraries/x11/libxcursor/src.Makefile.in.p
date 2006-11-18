--- src/Makefile.in.old	2006-11-12 12:50:00.000000000 +0000
+++ src/Makefile.in	2006-11-12 12:52:00.000000000 +0000
@@ -208,7 +208,7 @@
 #
 # Shared library version info.  This is not the same as the package version
 #
-libXcursor_la_LDFLAGS = -version-number 1:0:2 -no-undefined
+libXcursor_la_LDFLAGS = 
 libXcursorincludedir = $(includedir)/X11/Xcursor
 libXcursorinclude_HEADERS = $(top_srcdir)/include/X11/Xcursor/Xcursor.h 
 all: all-am
