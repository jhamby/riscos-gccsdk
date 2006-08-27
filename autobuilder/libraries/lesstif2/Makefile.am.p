--- Makefile.am.org	2006-08-27 20:42:27.000000000 +0200
+++ Makefile.am	2006-08-27 20:42:52.000000000 +0200
@@ -7,7 +7,7 @@
 		     missing mkinstalldirs \
 		     BUG-REPORTING INSTALL.txt ReleaseNotes.txt
 
-SUBDIRS	     = . scripts include lib clients doc @BuildTestDir@ autopackage
+SUBDIRS	     = . include lib
 
 DIST_SUBDIRS = scripts include lib clients doc test autopackage
 
