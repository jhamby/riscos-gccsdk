--- unix-zip.orig	2016-02-28 14:00:31.269568000 +0000
+++ unix-zip	2016-02-28 17:22:25.915217589 +0000
@@ -27,6 +27,8 @@
 done
 
 # Do the zip
-$ZIPCMD
+# For the autobuilder we let build program do the zip so leave it
+# for now and copy it to the correct place later
+# $ZIPCMD
 
 
