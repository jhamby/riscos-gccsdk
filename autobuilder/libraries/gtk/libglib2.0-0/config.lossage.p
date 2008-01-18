--- Makefile.in.orig	2005-03-05 11:58:16.000000000 +0000
+++ Makefile.in	2005-03-05 11:58:37.000000000 +0000
@@ -832,8 +832,7 @@
 install-exec-local: glibconfig.h
 	$(mkinstalldirs) $(DESTDIR)$(configexecincludedir)
 	file=$(DESTDIR)$(configexecincludedir)/glibconfig.h; \
-	if test -r $$file && cmp -s glibconfig.h $$file; then :; \
-	else $(INSTALL_DATA) glibconfig.h $$file; fi
+	$(INSTALL_DATA) glibconfig.h $$file
 
 uninstall-local:
 	rm -f $(DESTDIR)$(configexecincludedir)/glibconfig.h
