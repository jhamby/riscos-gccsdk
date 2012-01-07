--- libraries/tiff/configure.orig	2012-01-07 14:00:50.000000000 +0000
+++ libraries/tiff/configure	2012-01-07 14:01:13.000000000 +0000
@@ -36,9 +36,9 @@
 POSIXLY_CORRECT=1; export POSIXLY_CORRECT	# disable GNU extensions
 
 QUIET=no				# suppress messages to tty
-NONINTERACTIVE=no			# control prompting of config params
+NONINTERACTIVE=yes			# control prompting of config params
 SITE=					# dir where config.site is located
-TARGET=					# target system identity
+TARGET=arm-unknown-riscos					# target system identity
 SRCDIR=					# dir where source is located
 
 #
