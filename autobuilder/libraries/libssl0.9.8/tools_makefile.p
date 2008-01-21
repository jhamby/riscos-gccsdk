--- tools/Makefile.org	2006-04-19 18:32:54.010000000 +0200
+++ tools/Makefile	2006-04-19 18:33:22.760000000 +0200
@@ -22,15 +22,11 @@
 	@[ -n "$(INSTALLTOP)" ] # should be set by top Makefile...
 	@for i in $(APPS) ; \
 	do  \
-	(cp $$i $(INSTALL_PREFIX)$(INSTALLTOP)/bin/$$i.new; \
-	chmod 755 $(INSTALL_PREFIX)$(INSTALLTOP)/bin/$$i.new; \
-	mv -f $(INSTALL_PREFIX)$(INSTALLTOP)/bin/$$i.new $(INSTALL_PREFIX)$(INSTALLTOP)/bin/$$i ); \
+	(cp $$i $(INSTALL_PREFIX)$(INSTALLTOP)/bin/$$i ); \
 	done;
 	@for i in $(MISC_APPS) ; \
 	do  \
-	(cp $$i $(INSTALL_PREFIX)$(OPENSSLDIR)/misc/$$i.new; \
-	chmod 755 $(INSTALL_PREFIX)$(OPENSSLDIR)/misc/$$i.new; \
-	mv -f $(INSTALL_PREFIX)$(OPENSSLDIR)/misc/$$i.new $(INSTALL_PREFIX)$(OPENSSLDIR)/misc/$$i ); \
+	(cp $$i $(INSTALL_PREFIX)$(OPENSSLDIR)/misc/$$i ); \
 	done;

 files:
