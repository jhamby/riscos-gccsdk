--- makefile.orig	2009-12-03 13:06:52.000000000 -0800
+++ makefile	2009-12-03 13:09:24.000000000 -0800
@@ -30,7 +30,7 @@
 ifeq ($(OS),Windows_NT)
 DATA_DIR =
 else
-DATA_DIR = $(PREFIX)/share/games/edgar/
+DATA_DIR = "/<Edgar\$$Dir>/data/"
 endif
 
 ifeq ($(DEV),1)
