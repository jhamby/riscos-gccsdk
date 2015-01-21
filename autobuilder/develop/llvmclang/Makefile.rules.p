--- Makefile.rules.orig	2014-06-20 20:00:41.000000000 +0100
+++ Makefile.rules	2015-01-20 20:58:28.164843372 +0000
@@ -1434,6 +1434,11 @@
 	$(LLVMLibsOptions) $(ExtraLibs) $(TOOLLINKOPTSB) $(LIBS)
 	$(Echo) ======= Finished Linking $(BuildMode) Executable $(TOOLNAME) \
           $(StripWarnMsg)
+ifeq ($(TOOLNAME),clang)
+	cp -T $@ $(GCCSDK_INSTALL_CROSSBIN)/arm-unknown-riscos-clang
+	ln -T -s -f $(GCCSDK_INSTALL_CROSSBIN)/arm-unknown-riscos-clang \
+		    $(GCCSDK_INSTALL_CROSSBIN)/arm-unknown-riscos-clang++
+endif
 endif
 
 ifneq ($(strip $(ToolAliasBuildPath)),)
