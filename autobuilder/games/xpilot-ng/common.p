--- src/common/Makefile.in.orig	2005-09-16 01:35:54.000000000 +0200
+++ src/common/Makefile.in	2005-10-31 16:20:16.000000000 +0100
@@ -150,7 +150,7 @@
 XP_AUTHORS = @XP_AUTHORS@
 XP_COPYRIGHT = @XP_COPYRIGHT@
 XP_COPYRIGHTDATE = @XP_COPYRIGHTDATE@
-XP_DATADIR = @XP_DATADIR@
+XP_DATADIR = \<XPilot-NG\$$Dir\>/data
 XP_RELEASEDATE = @XP_RELEASEDATE@
 X_CFLAGS = @X_CFLAGS@
 X_EXTENSIONS_LIB = @X_EXTENSIONS_LIB@
@@ -199,7 +199,7 @@
 target_cpu = @target_cpu@
 target_os = @target_os@
 target_vendor = @target_vendor@
-AM_CPPFLAGS = -DCONF_DATADIR=\"$(pkgdatadir)/\"
+AM_CPPFLAGS = -DCONF_DATADIR=\"\<XPilot-NG\$$Dir\>/data/\"
 noinst_LIBRARIES = libxpcommon.a
 libxpcommon_a_SOURCES = \
 	arraylist.c arraylist.h astershape.h audio.h \
