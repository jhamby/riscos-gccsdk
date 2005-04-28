--- gtk/Makefile.in.orig	2005-04-28 11:09:46.000000000 +0100
+++ gtk/Makefile.in	2005-04-28 11:10:21.000000000 +0100
@@ -569,8 +569,7 @@
 install-exec-am: install-libLTLIBRARIES
 install-exec: install-exec-am
 
-install-data-am: install-gtkconfDATA install-libgtkincludeHEADERS \
-		install-data-local
+install-data-am: install-gtkconfDATA install-libgtkincludeHEADERS
 install-data: install-data-am
 
 install-am: all-am
