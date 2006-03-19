--- dist/Makefile.in.old	2003-04-25 12:53:41.000000000 +0100
+++ dist/Makefile.in	2003-04-25 11:54:34.000000000 +0100
@@ -469,7 +469,7 @@
 	@cd $(DESTDIR)$(bindir) && $(rm) -f $(PROGS)
 	@$(INSTALLER) -fp $(PROGS) $(DESTDIR)$(bindir)
 	@cd $(DESTDIR)$(bindir) && (test ! -f $(strip) || $(strip) $(PROGS))
-	@cd $(DESTDIR)$(bindir) && $(chmod) $(emode) $(PROGS)
+	#@cd $(DESTDIR)$(bindir) && $(chmod) $(emode) $(PROGS)

 uninstall_utilities:
 	-cd $(bindir) && $(rm) -f $(PROGS)
@@ -479,7 +479,7 @@
 	@test -d $(DESTDIR)$(docdir) || \
 	    ($(mkdir) -p $(DESTDIR)$(docdir) && $(chmod) $(dmode) $(DESTDIR)$(docdir))
 	@cd $(DESTDIR)$(docdir) && $(rm) -rf *
-	@cd $(srcdir)/docs && $(cp) -pr * $(DESTDIR)$(docdir)/
+	#@cd $(srcdir)/docs && $(cp) -pr * $(DESTDIR)$(docdir)/

 uninstall_docs:
 	-cd $(docdir) && $(rm) -rf *
--- dist/Makefile.in.old	2003-04-25 14:06:23.000000000 +0100
+++ dist/Makefile.in	2003-04-25 13:06:30.000000000 +0100
@@ -467,7 +467,7 @@
 	@test -d $(DESTDIR)$(bindir) || \
 	    ($(mkdir) -p $(DESTDIR)$(bindir) && $(chmod) $(dmode) $(DESTDIR)$(bindir))
 	@cd $(DESTDIR)$(bindir) && $(rm) -f $(PROGS)
-	@$(INSTALLER) -fp $(PROGS) $(DESTDIR)$(bindir)
+	@$(INSTALLER) -p $(PROGS) $(DESTDIR)$(bindir)
 	@cd $(DESTDIR)$(bindir) && (test ! -f $(strip) || $(strip) $(PROGS))
 	#@cd $(DESTDIR)$(bindir) && $(chmod) $(emode) $(PROGS)

