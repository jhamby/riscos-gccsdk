--- Makefile.config.in.orig	2008-01-06 22:07:27.000000000 -0800
+++ Makefile.config.in	2008-01-06 22:08:04.000000000 -0800
@@ -56,7 +56,7 @@
 # MAKE is set automatically by Make to what was used to invoke Make.
 
 # INSTALL must be autocreate the neccessary directories
-INSTALL = install -D
+INSTALL = $(GCCSDK_INSTALL_ENV)/ro-install -D
 # INSTALL = ginstall
 #Solaris:
 #INSTALL = /usr/ucb/install
