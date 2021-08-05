--- Makefile.orig	2021-08-03 23:09:50.671058841 -0700
+++ Makefile	2021-08-03 23:11:05.935384976 -0700
@@ -69,9 +69,9 @@
 AR = $(GCCSDK_INSTALL_CROSSBIN)/ar
 ARFLAGS = rc
 else
-CC = $(GCCSDK_INSTALL_CROSSBIN)/arm-unknown-riscos-gcc
+CC = $(GCCSDK_INSTALL_CROSSBIN)/arm-riscos-gnueabihf-gcc
 
-AR = $(GCCSDK_INSTALL_CROSSBIN)/arm-unknown-riscos-ar
+AR = $(GCCSDK_INSTALL_CROSSBIN)/arm-riscos-gnueabihf-ar
 ARFLAGS = rc
 
 %.o: %.c
