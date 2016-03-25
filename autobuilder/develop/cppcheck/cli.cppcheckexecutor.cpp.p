--- cli/cppcheckexecutor.cpp.orig	2016-03-25 09:23:20.510110487 +0000
+++ cli/cppcheckexecutor.cpp	2016-03-25 09:26:07.446112305 +0000
@@ -32,7 +32,7 @@
 #include <iostream>
 #include <sstream>
 
-#if !defined(NO_UNIX_SIGNAL_HANDLING) && defined(__GNUC__) && !defined(__CYGWIN__) && !defined(__MINGW32__) && !defined(__OS2__)
+#if !defined(NO_UNIX_SIGNAL_HANDLING) && defined(__GNUC__) && !defined(__CYGWIN__) && !defined(__MINGW32__) && !defined(__OS2__) && !defined(__riscos__)
 #define USE_UNIX_SIGNAL_HANDLING
 #include <cstdio>
 #include <signal.h>
@@ -50,7 +50,7 @@
 #endif
 #endif
 
-#if !defined(NO_UNIX_BACKTRACE_SUPPORT) && defined(USE_UNIX_SIGNAL_HANDLING) && defined(__GNUC__) && !defined(__CYGWIN__) && !defined(__MINGW32__) && !defined(__NetBSD__) && !defined(__SVR4) && !defined(__QNX__)
+#if !defined(NO_UNIX_BACKTRACE_SUPPORT) && defined(USE_UNIX_SIGNAL_HANDLING) && defined(__GNUC__) && !defined(__CYGWIN__) && !defined(__MINGW32__) && !defined(__NetBSD__) && !defined(__SVR4) && !defined(__QNX__) && !defined(__riscos__)
 #define USE_UNIX_BACKTRACE_SUPPORT
 #include <cxxabi.h>
 #include <execinfo.h>
