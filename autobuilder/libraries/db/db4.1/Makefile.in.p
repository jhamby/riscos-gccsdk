--- dist/Makefile.in.old	2003-04-06 18:02:50.000000000 +0100
+++ dist/Makefile.in	2003-04-06 17:02:54.000000000 +0100
@@ -585,15 +585,15 @@
 
 install_utilities:
 	echo "Installing DB utilities: $(bindir) ..."
-	@test -d $(bindir) || \
-	    ($(mkdir) -p $(bindir) && $(chmod) $(dmode) $(bindir))
-	@for i in $(UTIL_PROGS); do \
-		$(rm) -f $(bindir)/$$i $(bindir)/$$i.exe; \
-		test -f $$i.exe && i=$$i.exe || true; \
-		$(INSTALLER) $$i $(bindir)/$$i; \
-		test -f $(strip) && $(strip) $(bindir)/$$i || true; \
-		$(chmod) $(emode) $(bindir)/$$i; \
-	done
+#	@test -d $(bindir) || \
+#	    ($(mkdir) -p $(bindir) && $(chmod) $(dmode) $(bindir))
+#	@for i in $(UTIL_PROGS); do \
+#		$(rm) -f $(bindir)/$$i $(bindir)/$$i.exe; \
+#		test -f $$i.exe && i=$$i.exe || true; \
+#		$(INSTALLER) $$i $(bindir)/$$i; \
+#		test -f $(strip) && $(strip) $(bindir)/$$i || true; \
+#		$(chmod) $(emode) $(bindir)/$$i; \
+#	done
 
 uninstall_utilities:
 	@(cd $(bindir); for i in $(UTIL_PROGS); do \
