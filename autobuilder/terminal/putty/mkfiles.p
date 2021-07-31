--- mkfiles.pl.orig	2009-08-26 20:55:00.000000000 -0700
+++ mkfiles.pl	2009-08-26 20:55:22.000000000 -0700
@@ -931,7 +931,7 @@
     "# building with GTK 1.2, or you can set it to `pkg-config gtk+-2.0'\n".
     "# if you want to enforce 2.0. The default is to try 2.0 and fall back\n".
     "# to 1.2 if it isn't found.\n".
-    "GTK_CONFIG = sh -c 'pkg-config gtk+-2.0 \$\$0 2>/dev/null || gtk-config \$\$0'\n".
+    "GTK_CONFIG = sh -c 'ro-pkg-config gtk+-2.0 \$\$0 2>/dev/null || gtk-config \$\$0'\n".
     "\n".
     "-include Makefile.local\n".
     "\n".
