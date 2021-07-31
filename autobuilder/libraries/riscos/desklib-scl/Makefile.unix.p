--- !DLSources/Makefile.unix.orig	2012-11-22 18:55:16.044642720 +0000
+++ !DLSources/Makefile.unix	2012-11-22 18:57:51.796170082 +0000
@@ -80,7 +80,7 @@
 	mkdir -p $(GCCSDK_INSTALL_ENV)/include/DeskLib
 	$(GCCSDK_INSTALL_ENV)/ro-install $(DESKLIB_INCLUDE)/*.h $(DESKLIB_OLD_INCLUDE)/*.h $(GCCSDK_INSTALL_ENV)/include/DeskLib
 	mkdir -p $(GCCSDK_INSTALL_ENV)/lib
-	if [ "$(SCL)" == "true" ] ; then \
+	if [ "$(SCL)" ] ; then \
 	  $(GCCSDK_INSTALL_ENV)/ro-install $(DESKLIB) $(GCCSDK_INSTALL_ENV)/lib/libDesk$(SCLNAME).a; \
 	else \
 	  $(GCCSDK_INSTALL_ENV)/ro-install $(DESKLIB_STATIC) $(GCCSDK_INSTALL_ENV)/lib/libDesk.a; \
