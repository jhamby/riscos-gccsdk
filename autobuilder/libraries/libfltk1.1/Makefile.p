--- Makefile.orig	2008-01-12 20:28:04.000000000 -0800
+++ Makefile	2008-01-12 20:28:09.000000000 -0800
@@ -27,7 +27,7 @@
 
 include makeinclude
 
-DIRS	=	$(IMAGEDIRS) src fluid test documentation
+DIRS	=	$(IMAGEDIRS) src fluid #test documentation
 
 all: makeinclude
 	for dir in $(DIRS); do\
