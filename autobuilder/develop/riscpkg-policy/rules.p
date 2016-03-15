--- RiscPkg/Rules.orig	2016-03-15 12:43:29.013517000 +0000
+++ RiscPkg/Rules	2016-03-15 12:46:25.109519394 +0000
@@ -10,7 +10,8 @@
 	chmod -R =rX Temp/Manuals
 	make -C Temp -f ../RiscPkg/Rules $(PACKAGES)
 	chmod -R =rwX Temp/Manuals
-	rm -rf Temp
+# Leave files for autobuilder packaging step
+#	rm -rf Temp
 
 .PHONY: clean
 
@@ -20,4 +21,5 @@
 $(PACKAGES): %:
 	riscpkg-gencontrol $(firstword $(subst _, ,$@)) < ../RiscPkg/Control > RiscPkg/Control
 	rm -f ../RiscPkg/$@
-	zip -r -, ../RiscPkg/$@ * -i@../RiscPkg/$(firstword $(subst _, ,$@)).inc
+# Don't do zip as it's handled by the autobuilder
+#	zip -r -, ../RiscPkg/$@ * -i@../RiscPkg/$(firstword $(subst _, ,$@)).inc
