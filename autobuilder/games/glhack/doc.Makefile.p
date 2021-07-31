--- doc/Makefile.orig	2021-03-07 11:02:32.271502014 +0000
+++ doc/Makefile	2021-03-07 11:16:57.260825690 +0000
@@ -38,7 +38,7 @@
 
 
 GAME	= nethack
-MANDIR	= $(DESTDIR)$(PREFIX)/share/man/man$(MANEXT)
+MANDIR	= $GCCSDK_INSTALL_ENV/share/man/man$(MANEXT)
 MANEXT	= 6
 
 # manual installation for most BSD-style systems
