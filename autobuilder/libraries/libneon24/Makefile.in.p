--- Makefile.in.orig	2005-03-10 21:08:13.000000000 +0000
+++ Makefile.in	2005-03-10 21:08:01.000000000 +0000
@@ -109,7 +109,8 @@
 neon-config: neon-config.in
 	@./config.status neon-config
 
-install-docs: install-man install-html
+#install-docs: install-man install-html
+install-docs:
 
 install-html:
 	$(INSTALL) -d $(DESTDIR)$(docdir)/html
