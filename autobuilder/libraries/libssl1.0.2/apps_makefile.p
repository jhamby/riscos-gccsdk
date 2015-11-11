--- apps/Makefile.org	2005-07-05 20:41:28.000000000 +0200
+++ apps/Makefile	2006-04-19 18:15:32.820000000 +0200
@@ -102,20 +102,14 @@
 	@set -e; for i in $(EXE); \
 	do  \
 	(echo installing $$i; \
-	 cp $$i $(INSTALL_PREFIX)$(INSTALLTOP)/bin/$$i.new; \
-	 chmod 755 $(INSTALL_PREFIX)$(INSTALLTOP)/bin/$$i.new; \
-	 mv -f $(INSTALL_PREFIX)$(INSTALLTOP)/bin/$$i.new $(INSTALL_PREFIX)$(INSTALLTOP)/bin/$$i ); \
+	 cp $$i $(INSTALL_PREFIX)$(INSTALLTOP)/bin/$$i; ); \
 	 done;
 	@set -e; for i in $(SCRIPTS); \
 	do  \
 	(echo installing $$i; \
-	 cp $$i $(INSTALL_PREFIX)$(OPENSSLDIR)/misc/$$i.new; \
-	 chmod 755 $(INSTALL_PREFIX)$(OPENSSLDIR)/misc/$$i.new; \
-	 mv -f $(INSTALL_PREFIX)$(OPENSSLDIR)/misc/$$i.new $(INSTALL_PREFIX)$(OPENSSLDIR)/misc/$$i ); \
+	 cp $$i $(INSTALL_PREFIX)$(OPENSSLDIR)/misc/$$i ); \
 	 done
-	@cp openssl.cnf $(INSTALL_PREFIX)$(OPENSSLDIR)/openssl.cnf.new; \
-	chmod 644 $(INSTALL_PREFIX)$(OPENSSLDIR)/openssl.cnf.new; \
-	mv -f  $(INSTALL_PREFIX)$(OPENSSLDIR)/openssl.cnf.new $(INSTALL_PREFIX)$(OPENSSLDIR)/openssl.cnf
+	@cp openssl.cnf $(INSTALL_PREFIX)$(OPENSSLDIR)/openssl.cnf

 tags:
 	ctags $(SRC)
