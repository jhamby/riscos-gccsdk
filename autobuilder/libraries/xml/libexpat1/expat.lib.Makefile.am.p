--- expat/lib/Makefile.am.orig	2020-08-23 09:40:49.175344581 +0100
+++ expat/lib/Makefile.am	2020-08-23 09:41:14.110855886 +0100
@@ -50,7 +50,7 @@
     ../Changes
 
 install-data-hook:
-	cd "$(DESTDIR)$(docdir)" && $(am__mv) Changes changelog
+#	cd "$(DESTDIR)$(docdir)" && $(am__mv) Changes changelog
 
 uninstall-local:
 	$(RM) "$(DESTDIR)$(docdir)/changelog"
