--- RiscPkg/Rules.orig	2016-02-28 19:45:31.005244000 +0000
+++ RiscPkg/Rules	2016-02-28 19:49:38.618374311 +0000
@@ -42,14 +42,16 @@
 	cp !PackMan/!RunImage$(EXEEXT) $(INSTALL_TO)/!PackMan
 	cp !PackMan/Res$(RESEXT) $(INSTALL_TO)/!PackMan
 	make -C Temp -f ../RiscPkg/Rules $(PACKAGES)
-	rm -rf Temp
+        # Don't delete work directory for autobuilder as it's used later
+	# rm -rf Temp
 
 .PHONY: $(PACKAGES)
 $(PACKAGES): %:
 	$(TOOL_PATH)riscpkg-gencontrol $(firstword $(subst _, ,$@)) < ../RiscPkg/Control > RiscPkg/Control
 	$(FAKEPKGPREFIX)fakepkginst
 	rm -f ../RiscPkg/$@
-	$(ZIP) -r ^.RiscPkg.$(subst .,/,$@) * -i@^.RiscPkg.$(firstword $(subst _, ,$@))/inc
+	# Don't do zip in autobuilder as it's zipped up by build-program
+	# $(ZIP) -r ^.RiscPkg.$(subst .,/,$@) * -i@^.RiscPkg.$(firstword $(subst _, ,$@))/inc
 
 fakepkginst: fakepkgsrc/fakepkginst.cc
 	$(NATIVE_CXX) fakepkgsrc/fakepkginst.cc -ofakepkginst
