--- Makefile.am.orig	2010-08-29 13:04:11.000000000 +0100
+++ Makefile.am	2010-08-29 13:05:55.000000000 +0100
@@ -17,10 +17,10 @@
 #	cd scen; ../util/gladpack p ../@PACKAGE@-@VERSION@/levels.001 *.fss
 
 graphics.001:
-	cd pix; ../util/gladpack p ../graphics.001 *.pix
+	cd pix; ../util/gladpack$(AB_EXEEXT) p ../graphics.001 *.pix
 
 levels.001:
-	cd scen; ../util/gladpack p ../levels.001 *.fss
+	cd scen; ../util/gladpack$(AB_EXEEXT) p ../levels.001 *.fss
 
 install: install-recursive install-docs
 
