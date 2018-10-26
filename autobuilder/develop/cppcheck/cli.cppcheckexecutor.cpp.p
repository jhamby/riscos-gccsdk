--- ./cli/cppcheckexecutor.cpp.orig	2018-10-26 19:28:50.805601868 +0100
+++ ./cli/cppcheckexecutor.cpp	2018-10-26 19:30:30.203488775 +0100
@@ -44,7 +44,7 @@
 #include <utility>
 #include <vector>
 
-#if !defined(NO_UNIX_SIGNAL_HANDLING) && defined(__GNUC__) && !defined(__MINGW32__) && !defined(__OS2__)
+#if !defined(NO_UNIX_SIGNAL_HANDLING) && defined(__GNUC__) && !defined(__MINGW32__) && !defined(__OS2__) && !defined(__riscos__)
 #define USE_UNIX_SIGNAL_HANDLING
 #include <unistd.h>
 #if defined(__APPLE__)
@@ -61,7 +61,7 @@
 #endif
 #endif
 
-#if !defined(NO_UNIX_BACKTRACE_SUPPORT) && defined(USE_UNIX_SIGNAL_HANDLING) && defined(__GNUC__) && defined(__GLIBC__) && !defined(__CYGWIN__) && !defined(__MINGW32__) && !defined(__NetBSD__) && !defined(__SVR4) && !defined(__QNX__)
+#if !defined(NO_UNIX_BACKTRACE_SUPPORT) && defined(USE_UNIX_SIGNAL_HANDLING) && defined(__GNUC__) && defined(__GLIBC__) && !defined(__CYGWIN__) && !defined(__MINGW32__) && !defined(__NetBSD__) && !defined(__SVR4) && !defined(__QNX__) && !defined(__riscos__)
 #define USE_UNIX_BACKTRACE_SUPPORT
 #include <cxxabi.h>
 #include <execinfo.h>
