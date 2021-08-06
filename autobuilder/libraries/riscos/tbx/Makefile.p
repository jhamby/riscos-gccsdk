--- Makefile.orig	2021-08-05 16:53:07.435907756 -0700
+++ Makefile	2021-08-05 17:08:06.735772761 -0700
@@ -2,7 +2,7 @@
 
 # include FixDeps:Rules/make
 
-CXX=g++
+#CXX=g++
 CXXFLAGS=-O2 -Wall -mthrowback
 CPPFLAGS=-MMD -MP
 AR=ar
