--- Makefile.orig	2018-10-26 21:31:12.345151933 +0100
+++ Makefile	2018-10-26 21:34:01.294048009 +0100
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
@@ -250,7 +250,7 @@
 ###### Targets
 
 cppcheck: $(LIBOBJ) $(CLIOBJ) $(EXTOBJ)
-	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDFLAGS) $(RDYNAMIC)
+	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -static -o $@ $^ $(LIBS) $(LDFLAGS) $(RDYNAMIC)
 
 all:	cppcheck testrunner
 
@@ -330,7 +330,7 @@
 ###### Build
 
 $(SRCDIR)/analyzerinfo.o: lib/analyzerinfo.cpp lib/analyzerinfo.h lib/config.h lib/errorlogger.h lib/suppressions.h lib/importproject.h lib/platform.h lib/utils.h lib/path.h
-	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CFG) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(SRCDIR)/analyzerinfo.o $(SRCDIR)/analyzerinfo.cpp
+	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) -D_GLIBCXX_USE_C99 $(CFG) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(SRCDIR)/analyzerinfo.o $(SRCDIR)/analyzerinfo.cpp
 
 $(SRCDIR)/astutils.o: lib/astutils.cpp lib/astutils.h lib/errorlogger.h lib/config.h lib/suppressions.h lib/library.h lib/mathlib.h lib/standards.h lib/settings.h lib/importproject.h lib/platform.h lib/utils.h lib/timer.h lib/symboldatabase.h lib/token.h lib/valueflow.h
 	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CFG) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(SRCDIR)/astutils.o $(SRCDIR)/astutils.cpp
