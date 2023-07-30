--- Makefile.orig	2022-07-16 09:49:09.765722322 +0100
+++ Makefile	2022-07-16 10:12:38.265987159 +0100
@@ -37,7 +37,7 @@
     CPPFILESDIR=
 endif
 
-RDYNAMIC=-rdynamic
+RDYNAMIC=
 # Set the CPPCHK_GLIBCXX_DEBUG flag. This flag is not used in release Makefiles.
 # The _GLIBCXX_DEBUG define doesn't work in Cygwin or other Win32 systems.
 ifndef COMSPEC
@@ -281,7 +281,7 @@
 ###### Targets
 
 cppcheck: $(LIBOBJ) $(CLIOBJ) $(EXTOBJ)
-	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDFLAGS) $(RDYNAMIC)
+	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -static -o $@ $^ $(LIBS) $(LDFLAGS) $(RDYNAMIC)
 
 all:	cppcheck testrunner
 
@@ -391,7 +391,7 @@
 ###### Build
 
 $(libcppdir)/analyzerinfo.o: lib/analyzerinfo.cpp externals/tinyxml2/tinyxml2.h lib/analyzerinfo.h lib/color.h lib/config.h lib/errorlogger.h lib/errortypes.h lib/importproject.h lib/path.h lib/platform.h lib/suppressions.h lib/utils.h
-	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/analyzerinfo.o $(libcppdir)/analyzerinfo.cpp
+	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) -D_GLIBCXX_USE_C99 $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/analyzerinfo.o $(libcppdir)/analyzerinfo.cpp
 
 $(libcppdir)/astutils.o: lib/astutils.cpp lib/astutils.h lib/check.h lib/checkclass.h lib/config.h lib/errortypes.h lib/importproject.h lib/infer.h lib/library.h lib/mathlib.h lib/platform.h lib/settings.h lib/standards.h lib/suppressions.h lib/symboldatabase.h lib/templatesimplifier.h lib/timer.h lib/token.h lib/tokenize.h lib/tokenlist.h lib/utils.h lib/valueflow.h lib/valueptr.h
 	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/astutils.o $(libcppdir)/astutils.cpp
@@ -415,13 +415,13 @@
 	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/checkboost.o $(libcppdir)/checkboost.cpp
 
 $(libcppdir)/checkbufferoverrun.o: lib/checkbufferoverrun.cpp externals/tinyxml2/tinyxml2.h lib/astutils.h lib/check.h lib/checkbufferoverrun.h lib/color.h lib/config.h lib/ctu.h lib/errorlogger.h lib/errortypes.h lib/importproject.h lib/library.h lib/mathlib.h lib/platform.h lib/settings.h lib/standards.h lib/suppressions.h lib/symboldatabase.h lib/templatesimplifier.h lib/timer.h lib/token.h lib/tokenize.h lib/tokenlist.h lib/utils.h lib/valueflow.h
-	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/checkbufferoverrun.o $(libcppdir)/checkbufferoverrun.cpp
+	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) -D_GLIBCXX_USE_C99 $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/checkbufferoverrun.o $(libcppdir)/checkbufferoverrun.cpp
 
 $(libcppdir)/checkclass.o: lib/checkclass.cpp externals/tinyxml2/tinyxml2.h lib/astutils.h lib/check.h lib/checkclass.h lib/color.h lib/config.h lib/errorlogger.h lib/errortypes.h lib/importproject.h lib/library.h lib/mathlib.h lib/platform.h lib/settings.h lib/standards.h lib/suppressions.h lib/symboldatabase.h lib/templatesimplifier.h lib/timer.h lib/token.h lib/tokenize.h lib/tokenlist.h lib/utils.h lib/valueflow.h
-	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/checkclass.o $(libcppdir)/checkclass.cpp
+	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) -D_GLIBCXX_USE_C99 $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/checkclass.o $(libcppdir)/checkclass.cpp
 
 $(libcppdir)/checkcondition.o: lib/checkcondition.cpp lib/astutils.h lib/check.h lib/checkcondition.h lib/checkother.h lib/config.h lib/errortypes.h lib/importproject.h lib/library.h lib/mathlib.h lib/platform.h lib/settings.h lib/standards.h lib/suppressions.h lib/symboldatabase.h lib/templatesimplifier.h lib/timer.h lib/token.h lib/tokenize.h lib/tokenlist.h lib/utils.h lib/valueflow.h
-	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/checkcondition.o $(libcppdir)/checkcondition.cpp
+	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) -D_GLIBCXX_USE_C99 $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/checkcondition.o $(libcppdir)/checkcondition.cpp
 
 $(libcppdir)/checkexceptionsafety.o: lib/checkexceptionsafety.cpp lib/check.h lib/checkexceptionsafety.h lib/config.h lib/errortypes.h lib/importproject.h lib/library.h lib/mathlib.h lib/platform.h lib/settings.h lib/standards.h lib/suppressions.h lib/symboldatabase.h lib/templatesimplifier.h lib/timer.h lib/token.h lib/tokenize.h lib/tokenlist.h lib/utils.h lib/valueflow.h
 	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/checkexceptionsafety.o $(libcppdir)/checkexceptionsafety.cpp
@@ -436,7 +436,7 @@
 	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/checkio.o $(libcppdir)/checkio.cpp
 
 $(libcppdir)/checkleakautovar.o: lib/checkleakautovar.cpp lib/astutils.h lib/check.h lib/checkleakautovar.h lib/checkmemoryleak.h lib/checknullpointer.h lib/color.h lib/config.h lib/ctu.h lib/errorlogger.h lib/errortypes.h lib/importproject.h lib/library.h lib/mathlib.h lib/platform.h lib/settings.h lib/standards.h lib/suppressions.h lib/symboldatabase.h lib/templatesimplifier.h lib/timer.h lib/token.h lib/tokenize.h lib/tokenlist.h lib/utils.h lib/valueflow.h
-	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/checkleakautovar.o $(libcppdir)/checkleakautovar.cpp
+	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) -D_GLIBCXX_USE_C99 $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/checkleakautovar.o $(libcppdir)/checkleakautovar.cpp
 
 $(libcppdir)/checkmemoryleak.o: lib/checkmemoryleak.cpp lib/astutils.h lib/check.h lib/checkmemoryleak.h lib/color.h lib/config.h lib/errorlogger.h lib/errortypes.h lib/importproject.h lib/library.h lib/mathlib.h lib/platform.h lib/settings.h lib/standards.h lib/suppressions.h lib/symboldatabase.h lib/templatesimplifier.h lib/timer.h lib/token.h lib/tokenize.h lib/tokenlist.h lib/utils.h lib/valueflow.h
 	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/checkmemoryleak.o $(libcppdir)/checkmemoryleak.cpp
@@ -445,7 +445,7 @@
 	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/checknullpointer.o $(libcppdir)/checknullpointer.cpp
 
 $(libcppdir)/checkother.o: lib/checkother.cpp lib/astutils.h lib/check.h lib/checkclass.h lib/checkother.h lib/checkuninitvar.h lib/color.h lib/config.h lib/ctu.h lib/errorlogger.h lib/errortypes.h lib/importproject.h lib/library.h lib/mathlib.h lib/platform.h lib/settings.h lib/standards.h lib/suppressions.h lib/symboldatabase.h lib/templatesimplifier.h lib/timer.h lib/token.h lib/tokenize.h lib/tokenlist.h lib/utils.h lib/valueflow.h
-	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/checkother.o $(libcppdir)/checkother.cpp
+	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) -D_GLIBCXX_USE_C99 $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/checkother.o $(libcppdir)/checkother.cpp
 
 $(libcppdir)/checkpostfixoperator.o: lib/checkpostfixoperator.cpp lib/check.h lib/checkpostfixoperator.h lib/config.h lib/errortypes.h lib/importproject.h lib/library.h lib/mathlib.h lib/platform.h lib/settings.h lib/standards.h lib/suppressions.h lib/symboldatabase.h lib/templatesimplifier.h lib/timer.h lib/token.h lib/tokenize.h lib/tokenlist.h lib/utils.h lib/valueflow.h
 	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/checkpostfixoperator.o $(libcppdir)/checkpostfixoperator.cpp
@@ -460,7 +460,7 @@
 	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/checkstring.o $(libcppdir)/checkstring.cpp
 
 $(libcppdir)/checktype.o: lib/checktype.cpp lib/check.h lib/checktype.h lib/config.h lib/errortypes.h lib/importproject.h lib/library.h lib/mathlib.h lib/platform.h lib/settings.h lib/standards.h lib/suppressions.h lib/symboldatabase.h lib/templatesimplifier.h lib/timer.h lib/token.h lib/tokenize.h lib/tokenlist.h lib/utils.h lib/valueflow.h
-	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/checktype.o $(libcppdir)/checktype.cpp
+	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) -D_GLIBCXX_USE_C99 $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/checktype.o $(libcppdir)/checktype.cpp
 
 $(libcppdir)/checkuninitvar.o: lib/checkuninitvar.cpp lib/astutils.h lib/check.h lib/checknullpointer.h lib/checkuninitvar.h lib/color.h lib/config.h lib/ctu.h lib/errorlogger.h lib/errortypes.h lib/importproject.h lib/library.h lib/mathlib.h lib/platform.h lib/settings.h lib/standards.h lib/suppressions.h lib/symboldatabase.h lib/templatesimplifier.h lib/timer.h lib/token.h lib/tokenize.h lib/tokenlist.h lib/utils.h lib/valueflow.h
 	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/checkuninitvar.o $(libcppdir)/checkuninitvar.cpp
@@ -481,31 +481,31 @@
 	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/color.o $(libcppdir)/color.cpp
 
 $(libcppdir)/cppcheck.o: lib/cppcheck.cpp externals/picojson/picojson.h externals/simplecpp/simplecpp.h externals/tinyxml2/tinyxml2.h lib/analyzerinfo.h lib/check.h lib/checkunusedfunctions.h lib/clangimport.h lib/color.h lib/config.h lib/cppcheck.h lib/ctu.h lib/errorlogger.h lib/errortypes.h lib/importproject.h lib/library.h lib/mathlib.h lib/path.h lib/platform.h lib/preprocessor.h lib/settings.h lib/standards.h lib/suppressions.h lib/templatesimplifier.h lib/timer.h lib/token.h lib/tokenize.h lib/tokenlist.h lib/utils.h lib/valueflow.h lib/version.h
-	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/cppcheck.o $(libcppdir)/cppcheck.cpp
+	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) -D_GLIBCXX_USE_C99 $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/cppcheck.o $(libcppdir)/cppcheck.cpp
 
 $(libcppdir)/ctu.o: lib/ctu.cpp externals/tinyxml2/tinyxml2.h lib/astutils.h lib/check.h lib/color.h lib/config.h lib/ctu.h lib/errorlogger.h lib/errortypes.h lib/importproject.h lib/library.h lib/mathlib.h lib/platform.h lib/settings.h lib/standards.h lib/suppressions.h lib/symboldatabase.h lib/templatesimplifier.h lib/timer.h lib/token.h lib/tokenize.h lib/tokenlist.h lib/utils.h lib/valueflow.h
 	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/ctu.o $(libcppdir)/ctu.cpp
 
 $(libcppdir)/errorlogger.o: lib/errorlogger.cpp externals/tinyxml2/tinyxml2.h lib/analyzerinfo.h lib/check.h lib/color.h lib/config.h lib/cppcheck.h lib/errorlogger.h lib/errortypes.h lib/importproject.h lib/library.h lib/mathlib.h lib/path.h lib/platform.h lib/settings.h lib/standards.h lib/suppressions.h lib/templatesimplifier.h lib/timer.h lib/token.h lib/tokenlist.h lib/utils.h lib/valueflow.h
-	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/errorlogger.o $(libcppdir)/errorlogger.cpp
+	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) -D_GLIBCXX_USE_C99 $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/errorlogger.o $(libcppdir)/errorlogger.cpp
 
 $(libcppdir)/errortypes.o: lib/errortypes.cpp lib/config.h lib/errortypes.h
 	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/errortypes.o $(libcppdir)/errortypes.cpp
 
 $(libcppdir)/forwardanalyzer.o: lib/forwardanalyzer.cpp lib/analyzer.h lib/astutils.h lib/config.h lib/errortypes.h lib/forwardanalyzer.h lib/importproject.h lib/library.h lib/mathlib.h lib/platform.h lib/settings.h lib/standards.h lib/suppressions.h lib/symboldatabase.h lib/templatesimplifier.h lib/timer.h lib/token.h lib/utils.h lib/valueflow.h lib/valueptr.h
-	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/forwardanalyzer.o $(libcppdir)/forwardanalyzer.cpp
+	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) -D_GLIBCXX_USE_C99 $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/forwardanalyzer.o $(libcppdir)/forwardanalyzer.cpp
 
 $(libcppdir)/importproject.o: lib/importproject.cpp externals/picojson/picojson.h externals/simplecpp/simplecpp.h externals/tinyxml2/tinyxml2.h lib/config.h lib/errortypes.h lib/importproject.h lib/library.h lib/mathlib.h lib/path.h lib/platform.h lib/settings.h lib/standards.h lib/suppressions.h lib/templatesimplifier.h lib/timer.h lib/token.h lib/tokenize.h lib/tokenlist.h lib/utils.h lib/valueflow.h
 	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/importproject.o $(libcppdir)/importproject.cpp
 
 $(libcppdir)/infer.o: lib/infer.cpp lib/calculate.h lib/config.h lib/errortypes.h lib/infer.h lib/mathlib.h lib/valueflow.h lib/valueptr.h
-	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/infer.o $(libcppdir)/infer.cpp
+	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) -D_GLIBCXX_USE_C99 $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/infer.o $(libcppdir)/infer.cpp
 
 $(libcppdir)/library.o: lib/library.cpp externals/tinyxml2/tinyxml2.h lib/astutils.h lib/config.h lib/errortypes.h lib/library.h lib/mathlib.h lib/path.h lib/standards.h lib/symboldatabase.h lib/templatesimplifier.h lib/token.h lib/tokenlist.h lib/utils.h lib/valueflow.h
 	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/library.o $(libcppdir)/library.cpp
 
 $(libcppdir)/mathlib.o: lib/mathlib.cpp externals/simplecpp/simplecpp.h lib/config.h lib/errortypes.h lib/mathlib.h lib/utils.h
-	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/mathlib.o $(libcppdir)/mathlib.cpp
+	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) -D_GLIBCXX_USE_C99 $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/mathlib.o $(libcppdir)/mathlib.cpp
 
 $(libcppdir)/path.o: lib/path.cpp externals/simplecpp/simplecpp.h lib/config.h lib/path.h lib/utils.h
 	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/path.o $(libcppdir)/path.cpp
@@ -538,16 +538,16 @@
 	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/suppressions.o $(libcppdir)/suppressions.cpp
 
 $(libcppdir)/symboldatabase.o: lib/symboldatabase.cpp lib/astutils.h lib/color.h lib/config.h lib/errorlogger.h lib/errortypes.h lib/importproject.h lib/library.h lib/mathlib.h lib/platform.h lib/settings.h lib/standards.h lib/suppressions.h lib/symboldatabase.h lib/templatesimplifier.h lib/timer.h lib/token.h lib/tokenize.h lib/tokenlist.h lib/utils.h lib/valueflow.h
-	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/symboldatabase.o $(libcppdir)/symboldatabase.cpp
+	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) -D_GLIBCXX_USE_C99 $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/symboldatabase.o $(libcppdir)/symboldatabase.cpp
 
 $(libcppdir)/templatesimplifier.o: lib/templatesimplifier.cpp lib/color.h lib/config.h lib/errorlogger.h lib/errortypes.h lib/importproject.h lib/library.h lib/mathlib.h lib/platform.h lib/settings.h lib/standards.h lib/suppressions.h lib/templatesimplifier.h lib/timer.h lib/token.h lib/tokenize.h lib/tokenlist.h lib/utils.h lib/valueflow.h
-	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/templatesimplifier.o $(libcppdir)/templatesimplifier.cpp
+	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) -D_GLIBCXX_USE_C99 $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/templatesimplifier.o $(libcppdir)/templatesimplifier.cpp
 
 $(libcppdir)/timer.o: lib/timer.cpp lib/config.h lib/timer.h
 	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/timer.o $(libcppdir)/timer.cpp
 
 $(libcppdir)/token.o: lib/token.cpp lib/astutils.h lib/config.h lib/errortypes.h lib/importproject.h lib/library.h lib/mathlib.h lib/platform.h lib/settings.h lib/standards.h lib/suppressions.h lib/symboldatabase.h lib/templatesimplifier.h lib/timer.h lib/token.h lib/tokenlist.h lib/tokenrange.h lib/utils.h lib/valueflow.h
-	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/token.o $(libcppdir)/token.cpp
+	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) -D_GLIBCXX_USE_C99 $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/token.o $(libcppdir)/token.cpp
 
 $(libcppdir)/tokenize.o: lib/tokenize.cpp externals/simplecpp/simplecpp.h lib/check.h lib/color.h lib/config.h lib/errorlogger.h lib/errortypes.h lib/importproject.h lib/library.h lib/mathlib.h lib/platform.h lib/preprocessor.h lib/settings.h lib/standards.h lib/summaries.h lib/suppressions.h lib/symboldatabase.h lib/templatesimplifier.h lib/timer.h lib/token.h lib/tokenize.h lib/tokenlist.h lib/utils.h lib/valueflow.h
 	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/tokenize.o $(libcppdir)/tokenize.cpp
@@ -559,7 +559,7 @@
 	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/utils.o $(libcppdir)/utils.cpp
 
 $(libcppdir)/valueflow.o: lib/valueflow.cpp lib/analyzer.h lib/astutils.h lib/calculate.h lib/check.h lib/checkuninitvar.h lib/color.h lib/config.h lib/ctu.h lib/errorlogger.h lib/errortypes.h lib/forwardanalyzer.h lib/importproject.h lib/infer.h lib/library.h lib/mathlib.h lib/path.h lib/platform.h lib/programmemory.h lib/reverseanalyzer.h lib/settings.h lib/standards.h lib/suppressions.h lib/symboldatabase.h lib/templatesimplifier.h lib/timer.h lib/token.h lib/tokenlist.h lib/utils.h lib/valueflow.h lib/valueptr.h
-	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/valueflow.o $(libcppdir)/valueflow.cpp
+	$(CXX) ${INCLUDE_FOR_LIB} $(CPPFLAGS) -D_GLIBCXX_USE_C99 $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o $(libcppdir)/valueflow.o $(libcppdir)/valueflow.cpp
 
 cli/cmdlineparser.o: cli/cmdlineparser.cpp cli/cmdlineparser.h cli/cppcheckexecutor.h cli/filelister.h externals/tinyxml2/tinyxml2.h lib/check.h lib/color.h lib/config.h lib/errorlogger.h lib/errortypes.h lib/importproject.h lib/library.h lib/mathlib.h lib/path.h lib/platform.h lib/settings.h lib/standards.h lib/suppressions.h lib/timer.h lib/utils.h
 	$(CXX) ${INCLUDE_FOR_CLI} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o cli/cmdlineparser.o cli/cmdlineparser.cpp
@@ -568,7 +568,7 @@
 	$(CXX) ${INCLUDE_FOR_CLI} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o cli/cppcheckexecutor.o cli/cppcheckexecutor.cpp
 
 cli/filelister.o: cli/filelister.cpp cli/filelister.h lib/config.h lib/path.h lib/pathmatch.h lib/utils.h
-	$(CXX) ${INCLUDE_FOR_CLI} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o cli/filelister.o cli/filelister.cpp
+	$(CXX) ${INCLUDE_FOR_CLI} $(CPPFLAGS) -D_GLIBCXX_USE_C99 $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o cli/filelister.o cli/filelister.cpp
 
 cli/main.o: cli/main.cpp cli/cppcheckexecutor.h lib/color.h lib/config.h lib/errorlogger.h lib/errortypes.h lib/suppressions.h
 	$(CXX) ${INCLUDE_FOR_CLI} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o cli/main.o cli/main.cpp
@@ -766,7 +766,7 @@
 	$(CXX) ${INCLUDE_FOR_TEST} $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) $(UNDEF_STRICT_ANSI) -c -o test/testvarid.o test/testvarid.cpp
 
 externals/simplecpp/simplecpp.o: externals/simplecpp/simplecpp.cpp externals/simplecpp/simplecpp.h
-	$(CXX)  $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) -w $(UNDEF_STRICT_ANSI) -c -o externals/simplecpp/simplecpp.o externals/simplecpp/simplecpp.cpp
+	$(CXX)  $(CPPFLAGS) -D_GLIBCXX_USE_C99 $(CPPFILESDIR) $(CXXFLAGS) -w $(UNDEF_STRICT_ANSI) -c -o externals/simplecpp/simplecpp.o externals/simplecpp/simplecpp.cpp
 
 externals/tinyxml2/tinyxml2.o: externals/tinyxml2/tinyxml2.cpp externals/tinyxml2/tinyxml2.h
 	$(CXX)  $(CPPFLAGS) $(CPPFILESDIR) $(CXXFLAGS) -w $(UNDEF_STRICT_ANSI) -c -o externals/tinyxml2/tinyxml2.o externals/tinyxml2/tinyxml2.cpp
