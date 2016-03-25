--- Makefile.orig	2016-03-25 09:52:31.066129555 +0000
+++ Makefile	2016-03-25 09:53:36.870130271 +0000
@@ -19,12 +19,12 @@
 endif
 
 ifdef CFGDIR
-    CFG=-DCFGDIR=\"$(CFGDIR)\"
+    CFG=-DCFGDIR=\"\<Cppcheck\$$Dir\>\"
 else
     CFG=
 endif
 
-RDYNAMIC=-rdynamic
+RDYNAMIC=
 # Set the CPPCHK_GLIBCXX_DEBUG flag. This flag is not used in release Makefiles.
 # The _GLIBCXX_DEBUG define doesn't work in Cygwin or other Win32 systems.
 ifndef COMSPEC
@@ -247,7 +247,7 @@
 ###### Targets
 
 cppcheck: $(LIBOBJ) $(CLIOBJ) $(EXTOBJ)
-	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o cppcheck $(CLIOBJ) $(LIBOBJ) $(EXTOBJ) $(LIBS) $(LDFLAGS) $(RDYNAMIC)
+	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -static -o cppcheck $(CLIOBJ) $(LIBOBJ) $(EXTOBJ) $(LIBS) $(LDFLAGS) $(RDYNAMIC)
 
 all:	cppcheck testrunner
 
