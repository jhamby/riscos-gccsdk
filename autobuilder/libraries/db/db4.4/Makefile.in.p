--- dist/Makefile.in.orig	2006-04-20 01:15:14.000000000 +0200
+++ dist/Makefile.in	2006-04-20 01:41:59.230000000 +0200
@@ -913,7 +913,7 @@
 		test -f $$i.exe && i=$$i.exe || true; \
 		$(INSTALLER) $$i $(DESTDIR)$(bindir)/$$i; \
 		test -f $(strip) && $(strip) $(DESTDIR)$(bindir)/$$i || true; \
-		$(chmod) $(emode) $(DESTDIR)$(bindir)/$$i; \
+#		$(chmod) $(emode) $(DESTDIR)$(bindir)/$$i; \
 	done
 
 uninstall_utilities:
@@ -930,7 +930,7 @@
 	    ($(mkdir) -p $(DESTDIR)$(docdir) && \
 	    $(chmod) $(dmode) $(DESTDIR)$(docdir))
 	@cd $(DESTDIR)$(docdir) && $(rm) -rf $(DOCLIST)
-	@cd $(srcdir)/docs && $(cp) -pr $(DOCLIST) $(DESTDIR)$(docdir)/
+	@cd $(srcdir)/docs && $(cp) -p $(DOCLIST) $(DESTDIR)$(docdir)/
 
 uninstall_docs:
 	@cd $(DESTDIR)$(docdir) && $(rm) -rf $(DOCLIST)
