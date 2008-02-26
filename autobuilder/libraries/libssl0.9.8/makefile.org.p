--- Makefile.org.orig	2008-02-25 20:54:09.000000000 -0800
+++ Makefile.org	2008-02-25 20:59:05.000000000 -0800
@@ -125,7 +125,7 @@
 
 MAKEFILE= Makefile
 
-MANDIR=/usr/share/man
+MANDIR=$(GCCSDK_INSTALL_ENV)/man
 MAN1=1
 MAN3=3
 MANSUFFIX=ssl
@@ -498,10 +498,7 @@
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
