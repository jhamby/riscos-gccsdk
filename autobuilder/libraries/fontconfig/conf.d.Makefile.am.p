--- conf.d/Makefile.am.orig	2013-10-11 05:27:33.000000000 +0100
+++ conf.d/Makefile.am	2015-04-03 12:35:09.119523503 +0100
@@ -45,8 +45,8 @@
 EXTRA_DIST = $(template_DATA) $(DOC_SOURCES)
 CLEANFILES = $(DOC_FILES)
 
-configdir = $(CONFIGDIR)
-config_DATA = $(DOC_FILES)
+#configdir = $(CONFIGDIR)
+#config_DATA = $(DOC_FILES)
 
 templatedir = $(TEMPLATEDIR)
 template_DATA =				\
@@ -83,21 +83,21 @@
 README: $(srcdir)/README.in
 	sed "s|\@TEMPLATEDIR\@|$(templatedir)|" $< > $@
 
-install-data-hook:
-	mkdir -p $(DESTDIR)$(configdir)
-	@(echo cd $(DESTDIR)$(configdir);			\
-	  cd $(DESTDIR)$(configdir);				\
-	  for i in $(CONF_LINKS); do				\
-	    echo $(RM) $$i";" ln -s $(templatedir)/$$i .;	\
-	    $(RM) $$i;						\
-	    ln -s $(templatedir)/$$i .;				\
-	  done)
-uninstall-local:
-	@(echo cd $(DESTDIR)$(configdir);			\
-	  cd $(DESTDIR)$(configdir);				\
-	  for i in $(CONF_LINKS); do				\
-	    echo $(RM) $$i;					\
-	    $(RM) $$i;						\
-	  done)
+#install-data-hook:
+#	mkdir -p $(DESTDIR)$(configdir)
+#	@(echo cd $(DESTDIR)$(configdir);			\
+#	  cd $(DESTDIR)$(configdir);				\
+#	  for i in $(CONF_LINKS); do				\
+#	    echo $(RM) $$i";" ln -s $(templatedir)/$$i .;	\
+#	    $(RM) $$i;						\
+#	    ln -s $(templatedir)/$$i .;				\
+#	  done)
+#uninstall-local:
+#	@(echo cd $(DESTDIR)$(configdir);			\
+#	  cd $(DESTDIR)$(configdir);				\
+#	  for i in $(CONF_LINKS); do				\
+#	    echo $(RM) $$i;					\
+#	    $(RM) $$i;						\
+#	  done)
 
 -include $(top_srcdir)/git.mk
