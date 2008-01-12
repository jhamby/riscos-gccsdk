--- Makefile.in.orig	2008-01-12 12:13:34.000000000 -0800
+++ Makefile.in	2008-01-12 12:13:43.000000000 -0800
@@ -146,7 +146,7 @@
 sharedstatedir = @sharedstatedir@
 sysconfdir = @sysconfdir@
 target_alias = @target_alias@
-SUBDIRS = lib player plugin
+SUBDIRS = lib #player plugin
 
 EXTRA_DIST = autogen.sh
 subdir = .
