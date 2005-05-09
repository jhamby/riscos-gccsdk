Index: Makefile.in
===================================================================
RCS file: /cvsroot/rdesktop/rdesktop/Makefile.in,v
retrieving revision 1.8
diff -u -r1.8 Makefile.in
--- Makefile.in	4 May 2005 18:01:56 -0000	1.8
+++ Makefile.in	9 May 2005 08:50:57 -0000
@@ -11,7 +11,8 @@
 datadir     = @datadir@
 
 VERSION     = @PACKAGE_VERSION@
-KEYMAP_PATH = $(datadir)/rdesktop/keymaps/
+#KEYMAP_PATH = $(datadir)/rdesktop/keymaps/
+KEYMAP_PATH = \<RDesktop\$$Dir\>/keymaps/
 
 CC          = @CC@
 INSTALL     = @INSTALL@
