--- apps/Makefile.org	2006-02-14 03:22:23.000000000 +0100
+++ apps/Makefile	2006-02-14 03:21:34.000000000 +0100
@@ -102,9 +102,7 @@
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
