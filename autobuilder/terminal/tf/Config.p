--- unix/Config.old	2003-02-04 12:32:09.000000000 +0000
+++ unix/Config	2003-02-04 12:32:56.000000000 +0000
@@ -124,7 +124,7 @@
 # This is the place to add extra libraries as described in unix/README
 # if your linker can't resolve an external reference.
 
-LIBS=''
+LIBS='-L$GCCSDK_INSTALL_ENV/lib'
 
 
 ### Make Program.
