--- src/tools/qglobal.h.old	2003-01-20 12:41:19.000000000 +0000
+++ src/tools/qglobal.h	2003-01-20 16:46:22.000000000 +0000
@@ -149,6 +149,8 @@
 #elif defined(__svr4__) && defined(i386) /* Open UNIX 8 + GCC */
 #  define Q_OS_UNIXWARE
 #  define Q_OS_UNIXWARE7
+#elif defined(__riscos__)
+#define Q_OS_RISCOS
 #else
 #  error "Qt has not been ported to this OS - talk to qt-bugs@trolltech.com"
 #endif
@@ -264,6 +266,11 @@
 #elif defined(__GNUC__)
 #  define Q_CC_GNU
 #  define Q_C_CALLBACKS
+
+#  if defined(Q_OS_RISCOS)
+#    define Q_NO_EXPLICIT_KEYWORD
+#  endif
+
 #  if __GNUC__ == 2 && __GNUC_MINOR__ <= 7
 #    define Q_FULL_TEMPLATE_INSTANTIATION
 #  endif
@@ -544,7 +551,6 @@
 #  define Q_WS_MAC
 #endif
 
-
 /*
    Some classes do not permit copies to be made of an object.
    These classes contains a private copy constructor and operator=
