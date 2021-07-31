--- Makefile.in.orig	2012-09-24 16:51:11.000000000 +0100
+++ Makefile.in	2012-09-24 16:51:39.000000000 +0100
@@ -472,7 +472,7 @@
 	|| { echo "Gtk+Tests:ERROR: Failed to start Xvfb environment for X11 target tests."; exit 1; } \
 	&& DISPLAY=:$$XID && export DISPLAY
 
-SRC_SUBDIRS = gdk gtk modules demos tests perf
+SRC_SUBDIRS = gdk gtk modules #demos tests perf
 SUBDIRS = po po-properties $(SRC_SUBDIRS) docs m4macros build
 
 # require automake 1.4
