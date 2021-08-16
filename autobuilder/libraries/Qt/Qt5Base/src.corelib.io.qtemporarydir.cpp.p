--- ./src/corelib/io/qtemporarydir.cpp.orig	2021-08-14 22:19:58.468129377 -0700
+++ ./src/corelib/io/qtemporarydir.cpp	2021-08-14 22:37:20.849197536 -0700
@@ -44,7 +44,7 @@
 #include "qcoreapplication.h"
 #endif
 
-#if !defined(Q_OS_QNX) && !defined(Q_OS_WIN) &&!defined(Q_OS_ANDROID)
+#if !defined(Q_OS_QNX) && !defined(Q_OS_WIN) &&!defined(Q_OS_ANDROID) && !defined(Q_OS_RISCOS)
 #  define USE_SYSTEM_MKDTEMP
 #endif
 
