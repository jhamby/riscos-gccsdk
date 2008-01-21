--- Makefile.org.org	2006-04-19 18:35:30.000000000 +0200
+++ Makefile.org	2006-04-19 18:45:37.730000000 +0200
@@ -477,10 +477,7 @@
 	do \
 		if [ -f "$$i" ]; then \
 		(       echo installing $$i; \
-			cp $$i $(INSTALL_PREFIX)$(INSTALLTOP)/lib/$$i.new; \
-			$(RANLIB) $(INSTALL_PREFIX)$(INSTALLTOP)/lib/$$i.new; \
-			chmod 644 $(INSTALL_PREFIX)$(INSTALLTOP)/lib/$$i.new; \
-			mv -f $(INSTALL_PREFIX)$(INSTALLTOP)/lib/$$i.new $(INSTALL_PREFIX)$(INSTALLTOP)/lib/$$i ); \
+			cp $$i $(INSTALL_PREFIX)$(INSTALLTOP)/lib/$$i ); \
 		fi; \
 	done;
 	@set -e; if [ -n "$(SHARED_LIBS)" ]; then \
