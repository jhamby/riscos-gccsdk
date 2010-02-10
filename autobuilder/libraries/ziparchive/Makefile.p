--- _projects/Linux/Makefile.orig	2010-02-04 09:45:00.996296700 +0000
+++ _projects/Linux/Makefile	2010-02-04 10:41:51.244302400 +0000
@@ -10,9 +10,11 @@
 #otherwise, the system's bzip2 library will be used
 #INTERNAL_BZIP2 = 1
 
-CC=g++
+CCC:=$(CC)
+CC:=$(CXX)
+# CC=g++
 
-CCC=cc
+# CCC=cc
 #CCC=gcc
 	
 
@@ -39,7 +41,8 @@
 INCLUDES += -Ibzip2
 endif
 
-prefix = /usr
+#prefix = /usr
+prefix = $(GCCSDK_INSTALL_ENV)
 #prefix = 
 
 libdir = ${prefix}/lib
