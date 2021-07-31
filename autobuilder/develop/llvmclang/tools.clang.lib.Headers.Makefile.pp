--- tools/clang/lib/Headers/Makefile.orig	2014-04-17 01:52:48.000000000 +0100
+++ tools/clang/lib/Headers/Makefile	2014-10-30 20:14:07.484751721 +0000
@@ -41,23 +41,35 @@
 all-local:: $(OBJHEADERS) $(HeaderDir)/module.modulemap
 
 PROJ_headers := $(DESTDIR)$(PROJ_prefix)/lib/clang/$(CLANG_VERSION)/include
+RISCOS_headers := $(GCCSDK_INSTALL_CROSSBIN)/../lib/clang/$(CLANG_VERSION)/include
 
 INSTHEADERS := $(addprefix $(PROJ_headers)/, $(HEADERS))
 INSTHEADERS += $(PROJ_headers)/arm_neon.h
 
+INSTHEADERS_RISCOS := $(addprefix $(RISCOS_headers)/, $(HEADERS))
+INSTHEADERS_RISCOS += $(RISCOS_headers)/arm_neon.h
+
 $(PROJ_headers):
 	$(Verb) $(MKDIR) $@
+$(RISCOS_headers):
+	$(Verb) $(MKDIR) $@
 
 $(INSTHEADERS): $(PROJ_headers)/%.h: $(HeaderDir)/%.h | $(PROJ_headers)
 	$(Verb) $(DataInstall) $< $(PROJ_headers)
 	$(Echo) Installing compiler include file: $(notdir $<)
+	echo $(Verb) $(DataInstall) $< $(PROJ_headers)
+
+$(INSTHEADERS_RISCOS): $(RISCOS_headers)/%.h: $(HeaderDir)/%.h | $(RISCOS_headers)
+	$(verb) $(DataInstall) $< $(RISCOS_headers)
+	$(Echo) Installing compiler include file \(for RISC OS\): $(notdir $<)
+	echo $(Verb) $(DataInstall) $< $(RISCOS_headers)
 
 $(PROJ_headers)/module.modulemap: $(HeaderDir)/module.modulemap | $(PROJ_headers)
 	$(Verb) $(DataInstall) $< $(PROJ_headers)
 	$(Echo) Installing compiler module map file: $(notdir $<)
 
 
-install-local:: $(INSTHEADERS) $(PROJ_headers)/module.modulemap
+install-local:: $(INSTHEADERS) $(INSTHEADERS_RISCOS) $(PROJ_headers)/module.modulemap
 
 $(ObjDir)/arm_neon.h.inc.tmp : $(CLANG_LEVEL)/include/clang/Basic/arm_neon.td $(CLANG_TBLGEN) $(ObjDir)/.dir
 	$(Echo) "Building Clang arm_neon.h.inc with tblgen"
