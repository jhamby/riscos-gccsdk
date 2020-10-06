--- Makefile.orig	2020-10-06 10:31:24.207775991 +0100
+++ Makefile	2020-10-06 10:41:45.555366475 +0100
@@ -43,7 +43,7 @@
     CPPFILESDIR=
 endif
 
-RDYNAMIC=-rdynamic
+RDYNAMIC=
 # Set the CPPCHK_GLIBCXX_DEBUG flag. This flag is not used in release Makefiles.
 # The _GLIBCXX_DEBUG define doesn't work in Cygwin or other Win32 systems.
 ifndef COMSPEC
@@ -292,7 +292,7 @@
 ###### Targets
 
 cppcheck: $(LIBOBJ) $(CLIOBJ) $(EXTOBJ)
-	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDFLAGS) $(RDYNAMIC)
+	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -static -o $@ $^ $(LIBS) $(LDFLAGS) $(RDYNAMIC)
 
 all:	cppcheck testrunner
 
@@ -404,13 +404,13 @@
 ###### Build
 
 $(libcppdir)/analyzerinfo.o: lib/analyzerinfo.cpp externals/tinyxml/tinyxml2.h lib/analyzerinfo.h lib/config.h lib/errorlogger.h lib/errortypes.h lib/importproject.h lib/path.h lib/platform.h lib/suppressions.h lib/utils.h
-	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/analyzerinfo.o $(libcppdir)/analyzerinfo.cpp
+	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) -D_GLIBCXX_USE_C99 $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/analyzerinfo.o $(libcppdir)/analyzerinfo.cpp
 
 $(libcppdir)/astutils.o: lib/astutils.cpp lib/astutils.h lib/config.h lib/errortypes.h lib/importproject.h lib/library.h lib/mathlib.h lib/platform.h lib/settings.h lib/standards.h lib/suppressions.h lib/symboldatabase.h lib/templatesimplifier.h lib/timer.h lib/token.h lib/utils.h lib/valueflow.h
 	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/astutils.o $(libcppdir)/astutils.cpp
 
 $(libcppdir)/bughuntingchecks.o: lib/bughuntingchecks.cpp lib/astutils.h lib/bughuntingchecks.h lib/config.h lib/errorlogger.h lib/errortypes.h lib/exprengine.h lib/importproject.h lib/library.h lib/mathlib.h lib/platform.h lib/settings.h lib/standards.h lib/suppressions.h lib/symboldatabase.h lib/templatesimplifier.h lib/timer.h lib/token.h lib/utils.h lib/valueflow.h
-	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/bughuntingchecks.o $(libcppdir)/bughuntingchecks.cpp
+	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) -D_GLIBCXX_USE_C99 $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/bughuntingchecks.o $(libcppdir)/bughuntingchecks.cpp
 
 $(libcppdir)/check.o: lib/check.cpp lib/check.h lib/config.h lib/errorlogger.h lib/errortypes.h lib/importproject.h lib/library.h lib/mathlib.h lib/platform.h lib/settings.h lib/standards.h lib/suppressions.h lib/templatesimplifier.h lib/timer.h lib/token.h lib/tokenize.h lib/tokenlist.h lib/utils.h lib/valueflow.h
 	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/check.o $(libcppdir)/check.cpp
@@ -452,7 +452,7 @@
 	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/checkio.o $(libcppdir)/checkio.cpp
 
 $(libcppdir)/checkleakautovar.o: lib/checkleakautovar.cpp lib/astutils.h lib/check.h lib/checkleakautovar.h lib/checkmemoryleak.h lib/checknullpointer.h lib/config.h lib/ctu.h lib/errorlogger.h lib/errortypes.h lib/importproject.h lib/library.h lib/mathlib.h lib/platform.h lib/settings.h lib/standards.h lib/suppressions.h lib/symboldatabase.h lib/templatesimplifier.h lib/timer.h lib/token.h lib/tokenize.h lib/tokenlist.h lib/utils.h lib/valueflow.h
-	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/checkleakautovar.o $(libcppdir)/checkleakautovar.cpp
+	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) -D_GLIBCXX_USE_C99 $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/checkleakautovar.o $(libcppdir)/checkleakautovar.cpp
 
 $(libcppdir)/checkmemoryleak.o: lib/checkmemoryleak.cpp lib/astutils.h lib/check.h lib/checkmemoryleak.h lib/config.h lib/errorlogger.h lib/errortypes.h lib/importproject.h lib/library.h lib/mathlib.h lib/platform.h lib/settings.h lib/standards.h lib/suppressions.h lib/symboldatabase.h lib/templatesimplifier.h lib/timer.h lib/token.h lib/tokenize.h lib/tokenlist.h lib/utils.h lib/valueflow.h
 	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/checkmemoryleak.o $(libcppdir)/checkmemoryleak.cpp
@@ -461,7 +461,7 @@
 	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/checknullpointer.o $(libcppdir)/checknullpointer.cpp
 
 $(libcppdir)/checkother.o: lib/checkother.cpp lib/astutils.h lib/check.h lib/checkother.h lib/checkuninitvar.h lib/config.h lib/ctu.h lib/errorlogger.h lib/errortypes.h lib/importproject.h lib/library.h lib/mathlib.h lib/platform.h lib/settings.h lib/standards.h lib/suppressions.h lib/symboldatabase.h lib/templatesimplifier.h lib/timer.h lib/token.h lib/tokenize.h lib/tokenlist.h lib/utils.h lib/valueflow.h
-	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/checkother.o $(libcppdir)/checkother.cpp
+	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) -D_GLIBCXX_USE_C99 $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/checkother.o $(libcppdir)/checkother.cpp
 
 $(libcppdir)/checkpostfixoperator.o: lib/checkpostfixoperator.cpp lib/check.h lib/checkpostfixoperator.h lib/config.h lib/errortypes.h lib/importproject.h lib/library.h lib/mathlib.h lib/platform.h lib/settings.h lib/standards.h lib/suppressions.h lib/symboldatabase.h lib/templatesimplifier.h lib/timer.h lib/token.h lib/tokenize.h lib/tokenlist.h lib/utils.h lib/valueflow.h
 	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/checkpostfixoperator.o $(libcppdir)/checkpostfixoperator.cpp
@@ -494,7 +494,7 @@
 	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/clangimport.o $(libcppdir)/clangimport.cpp
 
 $(libcppdir)/cppcheck.o: lib/cppcheck.cpp externals/picojson.h externals/simplecpp/simplecpp.h externals/tinyxml/tinyxml2.h lib/analyzerinfo.h lib/check.h lib/checkunusedfunctions.h lib/clangimport.h lib/config.h lib/cppcheck.h lib/ctu.h lib/errorlogger.h lib/errortypes.h lib/exprengine.h lib/importproject.h lib/library.h lib/mathlib.h lib/path.h lib/platform.h lib/preprocessor.h lib/settings.h lib/standards.h lib/suppressions.h lib/templatesimplifier.h lib/timer.h lib/token.h lib/tokenize.h lib/tokenlist.h lib/utils.h lib/valueflow.h lib/version.h
-	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/cppcheck.o $(libcppdir)/cppcheck.cpp
+	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) -D_GLIBCXX_USE_C99 $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/cppcheck.o $(libcppdir)/cppcheck.cpp
 
 $(libcppdir)/ctu.o: lib/ctu.cpp externals/tinyxml/tinyxml2.h lib/astutils.h lib/check.h lib/config.h lib/ctu.h lib/errorlogger.h lib/errortypes.h lib/importproject.h lib/library.h lib/mathlib.h lib/platform.h lib/settings.h lib/standards.h lib/suppressions.h lib/symboldatabase.h lib/templatesimplifier.h lib/timer.h lib/token.h lib/tokenize.h lib/tokenlist.h lib/utils.h lib/valueflow.h
 	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/ctu.o $(libcppdir)/ctu.cpp
@@ -506,10 +506,10 @@
 	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/errortypes.o $(libcppdir)/errortypes.cpp
 
 $(libcppdir)/exprengine.o: lib/exprengine.cpp lib/astutils.h lib/bughuntingchecks.h lib/config.h lib/errorlogger.h lib/errortypes.h lib/exprengine.h lib/importproject.h lib/library.h lib/mathlib.h lib/platform.h lib/settings.h lib/standards.h lib/suppressions.h lib/symboldatabase.h lib/templatesimplifier.h lib/timer.h lib/token.h lib/tokenize.h lib/tokenlist.h lib/utils.h lib/valueflow.h
-	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/exprengine.o $(libcppdir)/exprengine.cpp
+	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) -D_GLIBCXX_USE_C99$(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/exprengine.o $(libcppdir)/exprengine.cpp
 
 $(libcppdir)/forwardanalyzer.o: lib/forwardanalyzer.cpp lib/astutils.h lib/config.h lib/errortypes.h lib/forwardanalyzer.h lib/importproject.h lib/library.h lib/mathlib.h lib/platform.h lib/settings.h lib/standards.h lib/suppressions.h lib/symboldatabase.h lib/templatesimplifier.h lib/timer.h lib/token.h lib/utils.h lib/valueflow.h lib/valueptr.h
-	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/forwardanalyzer.o $(libcppdir)/forwardanalyzer.cpp
+	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) -D_GLIBCXX_USE_C99 $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/forwardanalyzer.o $(libcppdir)/forwardanalyzer.cpp
 
 $(libcppdir)/importproject.o: lib/importproject.cpp externals/picojson.h externals/tinyxml/tinyxml2.h lib/config.h lib/errortypes.h lib/importproject.h lib/library.h lib/mathlib.h lib/path.h lib/platform.h lib/settings.h lib/standards.h lib/suppressions.h lib/templatesimplifier.h lib/timer.h lib/token.h lib/tokenize.h lib/tokenlist.h lib/utils.h lib/valueflow.h
 	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/importproject.o $(libcppdir)/importproject.cpp
@@ -545,10 +545,10 @@
 	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/suppressions.o $(libcppdir)/suppressions.cpp
 
 $(libcppdir)/symboldatabase.o: lib/symboldatabase.cpp lib/astutils.h lib/config.h lib/errorlogger.h lib/errortypes.h lib/importproject.h lib/library.h lib/mathlib.h lib/platform.h lib/settings.h lib/standards.h lib/suppressions.h lib/symboldatabase.h lib/templatesimplifier.h lib/timer.h lib/token.h lib/tokenize.h lib/tokenlist.h lib/utils.h lib/valueflow.h
-	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/symboldatabase.o $(libcppdir)/symboldatabase.cpp
+	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) -D_GLIBCXX_USE_C99 $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/symboldatabase.o $(libcppdir)/symboldatabase.cpp
 
 $(libcppdir)/templatesimplifier.o: lib/templatesimplifier.cpp lib/config.h lib/errorlogger.h lib/errortypes.h lib/importproject.h lib/library.h lib/mathlib.h lib/platform.h lib/settings.h lib/standards.h lib/suppressions.h lib/templatesimplifier.h lib/timer.h lib/token.h lib/tokenize.h lib/tokenlist.h lib/utils.h lib/valueflow.h
-	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/templatesimplifier.o $(libcppdir)/templatesimplifier.cpp
+	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) -D_GLIBCXX_USE_C99 $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/templatesimplifier.o $(libcppdir)/templatesimplifier.cpp
 
 $(libcppdir)/timer.o: lib/timer.cpp lib/config.h lib/timer.h
 	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/timer.o $(libcppdir)/timer.cpp
@@ -566,7 +566,7 @@
 	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/utils.o $(libcppdir)/utils.cpp
 
 $(libcppdir)/valueflow.o: lib/valueflow.cpp lib/astutils.h lib/config.h lib/errorlogger.h lib/errortypes.h lib/forwardanalyzer.h lib/importproject.h lib/library.h lib/mathlib.h lib/path.h lib/platform.h lib/programmemory.h lib/settings.h lib/standards.h lib/suppressions.h lib/symboldatabase.h lib/templatesimplifier.h lib/timer.h lib/token.h lib/tokenlist.h lib/utils.h lib/valueflow.h lib/valueptr.h
-	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/valueflow.o $(libcppdir)/valueflow.cpp
+	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) -D_GLIBCXX_USE_C99 $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/valueflow.o $(libcppdir)/valueflow.cpp
 
 cli/cmdlineparser.o: cli/cmdlineparser.cpp cli/cmdlineparser.h cli/cppcheckexecutor.h cli/filelister.h cli/threadexecutor.h externals/tinyxml/tinyxml2.h lib/check.h lib/config.h lib/errorlogger.h lib/errortypes.h lib/importproject.h lib/library.h lib/mathlib.h lib/path.h lib/platform.h lib/settings.h lib/standards.h lib/suppressions.h lib/timer.h lib/utils.h
 	$(CXX) ${INCLUDE_FOR_CLI} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o cli/cmdlineparser.o cli/cmdlineparser.cpp
