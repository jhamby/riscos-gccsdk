--- unix/Makefile.gtk.orig	2009-08-26 17:29:53.000000000 -0700
+++ unix/Makefile.gtk	2009-08-26 17:30:33.000000000 -0700
@@ -109,7 +109,7 @@
 # building with GTK 1.2, or you can set it to `pkg-config gtk+-2.0'
 # if you want to enforce 2.0. The default is to try 2.0 and fall back
 # to 1.2 if it isn't found.
-GTK_CONFIG = sh -c 'pkg-config gtk+-2.0 $$0 2>/dev/null || gtk-config $$0'
+GTK_CONFIG = sh -c 'ro-pkg-config gtk+-2.0 $$0 2>/dev/null || gtk-config $$0'
 
 -include Makefile.local
 
