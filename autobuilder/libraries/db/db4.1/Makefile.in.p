--- dist/Makefile.in.orig	2002-08-30 20:11:56.000000000 +0200
+++ dist/Makefile.in	2006-04-19 22:29:30.190000000 +0200
@@ -592,7 +592,7 @@
 		test -f $$i.exe && i=$$i.exe || true; \
 		$(INSTALLER) $$i $(bindir)/$$i; \
 		test -f $(strip) && $(strip) $(bindir)/$$i || true; \
-		$(chmod) $(emode) $(bindir)/$$i; \
+#		$(chmod) $(emode) $(bindir)/$$i; \
 	done

 uninstall_utilities:
@@ -608,7 +608,7 @@
 	@test -d $(docdir) || \
 	    ($(mkdir) -p $(docdir) && $(chmod) $(dmode) $(docdir))
 	@cd $(docdir) && $(rm) -rf $(DOCLIST)
-	@cd $(srcdir)/docs && $(cp) -pr $(DOCLIST) $(docdir)/
+	@cd $(srcdir)/docs && $(cp) -p $(DOCLIST) $(docdir)/

 uninstall_docs:
 	@cd $(docdir) && $(rm) -rf $(DOCLIST)
