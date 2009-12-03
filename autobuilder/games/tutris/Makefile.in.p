--- src/Makefile.in.orig	2009-12-03 14:59:36.000000000 -0800
+++ src/Makefile.in	2009-12-03 14:59:49.000000000 -0800
@@ -85,7 +85,7 @@
 
 bin_PROGRAMS = tutris
 
-datafiledir = $(datadir)/games/tutris
+datafiledir = $(datadir)
 AM_CPPFLAGS = -DDATA_DIR=\"$(datafiledir)/\"
 tutris_SOURCES = Main.cpp cTutris.hpp cTutris.cpp SoFont.h SoFont.cpp
 subdir = src
