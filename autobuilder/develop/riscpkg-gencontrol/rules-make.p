--- !RiscPkg-GenControl/Rules.make.orig	2015-02-03 12:55:29.667708000 +0000
+++ !RiscPkg-GenControl/Rules.make	2015-02-03 13:01:22.175865969 +0000
@@ -6,9 +6,9 @@
 
 CXX = gcc
 LD = gcc
-FIXDEPS = fixdeps
+#FIXDEPS = fixdeps
 
-CPPFLAGS = -Ilibpkg:
+CPPFLAGS = -Ilibpkg
 CXXFLAGS = -mthrowback -munixlib -mpoke-function-name -Wall -W -Wno-unused -O2
 
 .SUFFIXES: .o .cc .d .dd
@@ -16,10 +16,10 @@
 .cc.o:
 	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -o $@ $<
 
-.cc.dd:
-	$(CXX) -MM $(CPPFLAGS) $< > $@
+#.cc.dd:
+#	$(CXX) -MM $(CPPFLAGS) $< > $@
 
-.dd.d:
-	$(FIXDEPS) $* < $< > $@
+#.dd.d:
+#	$(FIXDEPS) $* < $< > $@
 
 .DELETE_ON_ERROR:
