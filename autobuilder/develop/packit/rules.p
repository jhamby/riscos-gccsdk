--- RiscPkg/Rules.orig	2017-03-30 13:05:37.441007999 +0100
+++ RiscPkg/Rules	2017-03-30 13:06:44.481007999 +0100
@@ -36,13 +36,15 @@
 	# Temporarily delete HTML pages - will remove from SVN with next release
 	rm -rf $(INSTALL_TO)/!PackIt/html
 	make -C Temp -f ../RiscPkg/Rules $(PACKAGES)
-	rm -rf Temp
-
+        # Don't delete work directory for autobuilder as it's used later
+	# rm -rf Temp
+ 
 .PHONY: $(PACKAGES)
 $(PACKAGES): %:
 	$(TOOL_PATH)riscpkg-gencontrol $(firstword $(subst _, ,$@)) < ../RiscPkg/Control > RiscPkg/Control
 	rm -f ../RiscPkg/$@
-	$(ZIP) -r ^.RiscPkg.$@ * -i@^.RiscPkg.$(firstword $(subst _, ,$@))/inc
-
+	# Don't do zip in autobuilder as it's zipped up by build-program
+	# $(ZIP) -r ^.RiscPkg.$@ * -i@^.RiscPkg.$(firstword $(subst _, ,$@))/inc
+ 
 build:
 	make
