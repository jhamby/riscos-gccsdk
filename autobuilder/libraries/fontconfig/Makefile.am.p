--- Makefile.am.orig	2013-10-11 05:27:33.000000000 +0100
+++ Makefile.am	2015-04-03 11:47:50.143486714 +0100
@@ -23,7 +23,7 @@
 
 SUBDIRS=fontconfig fc-case fc-lang fc-glyphname src \
 	fc-cache fc-cat fc-list fc-match fc-pattern fc-query fc-scan \
-	fc-validate conf.d test
+	fc-validate conf.d
 if ENABLE_DOCS
 SUBDIRS += doc
 endif
@@ -103,29 +103,29 @@
 		$(srcdir)/$@.in > $@.tmp && \
 	mv $@.tmp $@
 
-install-data-local: fonts.conf
-	$(mkinstalldirs) $(DESTDIR)$(baseconfigdir) $(DESTDIR)$(fc_cachedir)
-	if [ -f $(DESTDIR)$(baseconfigdir)/fonts.conf ]; then \
-	  echo "backing up existing $(DESTDIR)$(baseconfigdir)/fonts.conf"; \
-	  mv $(DESTDIR)$(baseconfigdir)/fonts.conf $(DESTDIR)$(baseconfigdir)/fonts.conf.bak; \
-	fi
-	if [ -f $(srcdir)/fonts.conf ]; then \
-	  echo " $(INSTALL_DATA) $(srcdir)/fonts.conf $(DESTDIR)$(baseconfigdir)/fonts.conf"; \
-	  $(INSTALL_DATA) $(srcdir)/fonts.conf $(DESTDIR)$(baseconfigdir)/fonts.conf; \
-	else if [ -f fonts.conf ]; then \
-	  echo " $(INSTALL_DATA) fonts.conf $(DESTDIR)$(baseconfigdir)/fonts.conf"; \
-	  $(INSTALL_DATA) fonts.conf $(DESTDIR)$(baseconfigdir)/fonts.conf; \
-	fi; fi
-	@(if $(RUN_FC_CACHE_TEST); then \
-	    echo "fc-cache$(EXEEXT) -s -f -v"; \
-	    fc-cache/fc-cache$(EXEEXT) -s -f -v; \
-	else \
-	    echo "***"; \
-	    echo "*** Warning: fonts.cache not built"; \
-	    echo "***"; \
-	    echo "*** Generate this file manually on host system using fc-cache"; \
-	    echo "***"; \
-	fi)
+#install-data-local: fonts.conf
+#	$(mkinstalldirs) $(DESTDIR)$(baseconfigdir) $(DESTDIR)$(fc_cachedir)
+#	if [ -f $(DESTDIR)$(baseconfigdir)/fonts.conf ]; then \
+#	  echo "backing up existing $(DESTDIR)$(baseconfigdir)/fonts.conf"; \
+#	  mv $(DESTDIR)$(baseconfigdir)/fonts.conf $(DESTDIR)$(baseconfigdir)/fonts.conf.bak; \
+#	fi
+#	if [ -f $(srcdir)/fonts.conf ]; then \
+#	  echo " $(INSTALL_DATA) $(srcdir)/fonts.conf $(DESTDIR)$(baseconfigdir)/fonts.conf"; \
+#	  $(INSTALL_DATA) $(srcdir)/fonts.conf $(DESTDIR)$(baseconfigdir)/fonts.conf; \
+#	else if [ -f fonts.conf ]; then \
+#	  echo " $(INSTALL_DATA) fonts.conf $(DESTDIR)$(baseconfigdir)/fonts.conf"; \
+#	  $(INSTALL_DATA) fonts.conf $(DESTDIR)$(baseconfigdir)/fonts.conf; \
+#	fi; fi
+#	@(if $(RUN_FC_CACHE_TEST); then \
+#	    echo "fc-cache$(EXEEXT) -s -f -v"; \
+#	    fc-cache/fc-cache$(EXEEXT) -s -f -v; \
+#	else \
+#	    echo "***"; \
+#	    echo "*** Warning: fonts.cache not built"; \
+#	    echo "***"; \
+#	    echo "*** Generate this file manually on host system using fc-cache"; \
+#	    echo "***"; \
+#	fi)
 
 uninstall-local:
 	if [ -f $(srcdir)/fonts.conf ]; then \
