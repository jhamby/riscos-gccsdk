--- Makefile.orig	2010-03-15 21:18:33.000000000 +0000
+++ Makefile	2010-03-15 21:30:38.000000000 +0000
@@ -5,7 +5,7 @@
 
 PREFIX = RTK-0.7.1
 
-all: bin doc dist
+all: bin doc install #dist
 
 bin:
 	make -C !RTK bin
@@ -17,7 +17,7 @@
 
 dist-src:
 	rm -f $(PREFIX).src.zip
-	rm -f $(PREFIX).!RTK
+	rm -f $(PREFIX).!RTK
 	rm -rf $(PREFIX)
 	mkdir $(PREFIX)
 	ln -s !RTK $(subst .,/,$(PREFIX)).!RTK
@@ -33,3 +33,20 @@
 dist-doc:
 	rm -f $(PREFIX).doc.zip
 	zip -r $(PREFIX).doc.zip !RTK -i@!RTK/include.doc
+
+install:
+	cp !RTK/rtk.a $(GCCSDK_INSTALL_ENV)/lib/librtk.a
+	mkdir -p $(GCCSDK_INSTALL_ENV)/include/rtk/util
+	cp !RTK/rtk/graphics/*.h $(GCCSDK_INSTALL_ENV)/include/rtk/util
+	mkdir -p $(GCCSDK_INSTALL_ENV)/include/rtk/graphics
+	cp !RTK/rtk/graphics/*.h $(GCCSDK_INSTALL_ENV)/include/rtk/graphics
+	mkdir -p $(GCCSDK_INSTALL_ENV)/include/rtk/os
+	cp !RTK/rtk/os/*.h $(GCCSDK_INSTALL_ENV)/include/rtk/os
+	mkdir -p $(GCCSDK_INSTALL_ENV)/include/rtk/desktop
+	cp !RTK/rtk/desktop/*.h $(GCCSDK_INSTALL_ENV)/include/rtk/desktop
+	mkdir -p $(GCCSDK_INSTALL_ENV)/include/rtk/events
+	cp !RTK/rtk/events/*.h $(GCCSDK_INSTALL_ENV)/include/rtk/events
+	mkdir -p $(GCCSDK_INSTALL_ENV)/include/rtk/transfer
+	cp !RTK/rtk/transfer/*.h $(GCCSDK_INSTALL_ENV)/include/rtk/transfer
+	mkdir -p $(GCCSDK_INSTALL_ENV)/include/rtk/swi
+	cp !RTK/rtk/swi/*.h $(GCCSDK_INSTALL_ENV)/include/rtk/swi
