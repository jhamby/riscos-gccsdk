--- Makefile.in.org	2006-08-27 20:53:44.000000000 +0200
+++ Makefile.in	2006-08-27 20:53:55.000000000 +0200
@@ -279,7 +279,7 @@
 		     missing mkinstalldirs \
 		     BUG-REPORTING INSTALL.txt ReleaseNotes.txt
 
-SUBDIRS = . scripts include lib clients doc @BuildTestDir@ autopackage
+SUBDIRS = . include lib
 DIST_SUBDIRS = scripts include lib clients doc test autopackage
 
 #
