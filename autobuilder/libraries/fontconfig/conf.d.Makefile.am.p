--- conf.d/Makefile.am.orig	2018-03-27 09:40:07.000000000 +0100
+++ conf.d/Makefile.am	2021-10-30 11:52:13.840359652 +0100
@@ -47,8 +47,8 @@
 EXTRA_DIST = $(template_DATA) $(DOC_SOURCES)
 CLEANFILES = $(DOC_FILES)
 
-configdir = $(CONFIGDIR)
-config_DATA = $(DOC_FILES)
+#configdir = $(CONFIGDIR)
+#config_DATA = $(DOC_FILES)
 
 templatedir = $(TEMPLATEDIR)
 template_DATA =				\
@@ -90,21 +90,21 @@
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
