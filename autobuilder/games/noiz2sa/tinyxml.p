--- src/bulletml/tinyxml/Makefile.orig	2009-08-03 07:08:25.000000000 -0700
+++ src/bulletml/tinyxml/Makefile	2009-08-03 07:10:14.000000000 -0700
@@ -15,9 +15,9 @@
 
 #****************************************************************************
 
-CC     := gcc
-CXX    := g++
-LD     := g++
+#CC     := gcc
+#CXX    := g++
+LD     := $(CXX)
 AR     := ar rc
 RANLIB := ranlib
 
