--- Makefile.orig	2008-01-12 20:28:04.000000000 -0800
+++ Makefile	2008-01-12 20:28:09.000000000 -0800
@@ -27,7 +27,7 @@
 
 include makeinclude
 
-DIRS	=	$(IMAGEDIRS) src fluid test documentation
+DIRS	=	$(IMAGEDIRS) src fluid #test documentation
 
 all: makeinclude
 	for dir in $(DIRS); do\
--- fluid/Makefile.orig	2009-07-26 13:30:09.000000000 -0700
+++ fluid/Makefile	2009-07-26 13:30:24.000000000 -0700
@@ -77,7 +77,7 @@
 fluid-shared$(EXEEXT):	$(OBJECTS) ../src/$(DSONAME) ../src/$(FLDSONAME) \
 			../src/$(IMGDSONAME)
 	echo Linking $@...
-	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ $(OBJECTS) \
+	$(CXX) $(ARCHFLAGS) $(LDFLAGS) -o $@ $(OBJECTS) $(LDLIBS) \
 	    -L../src -Wl,-rpath-link,../src -lfltk_images -lfltk_forms
 	$(POSTBUILD) $@ ../FL/mac.r
 
