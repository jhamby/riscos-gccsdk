--- Makefile.org.orig	2010-01-27 16:06:36.000000000 +0000
+++ Makefile.org	2010-06-04 20:56:24.498454186 +0100
@@ -153,7 +153,7 @@
 
 MAKEFILE= Makefile
 
-MANDIR=/usr/share/man
+MANDIR=$(GCCSDK_INSTALL_ENV)/man
 MAN1=1
 MAN3=3
 MANSUFFIX=
@@ -636,10 +636,7 @@
 	do \
 		if [ -f "$$i" ]; then \
 		(       echo installing $$i; \
-			cp $$i $(INSTALL_PREFIX)$(INSTALLTOP)/$(LIBDIR)/$$i.new; \
-			$(RANLIB) $(INSTALL_PREFIX)$(INSTALLTOP)/$(LIBDIR)/$$i.new; \
-			chmod 644 $(INSTALL_PREFIX)$(INSTALLTOP)/$(LIBDIR)/$$i.new; \
-			mv -f $(INSTALL_PREFIX)$(INSTALLTOP)/$(LIBDIR)/$$i.new $(INSTALL_PREFIX)$(INSTALLTOP)/$(LIBDIR)/$$i ); \
+			cp $$i $(INSTALL_PREFIX)$(INSTALLTOP)/lib/$$i ); \
 		fi; \
 	done;
 	@set -e; if [ -n "$(SHARED_LIBS)" ]; then \
