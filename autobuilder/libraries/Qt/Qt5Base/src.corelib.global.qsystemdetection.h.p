--- src/corelib/global/qsystemdetection.h.orig	2014-05-15 18:12:17.000000000 +0100
+++ src/corelib/global/qsystemdetection.h	2014-05-27 19:55:16.354522842 +0100
@@ -177,6 +177,9 @@
 #  define Q_OS_INTEGRITY
 #elif defined(VXWORKS) /* there is no "real" VxWorks define - this has to be set in the mkspec! */
 #  define Q_OS_VXWORKS
+#elif defined(__riscos__)
+#  define Q_OS_RISCOS
+#  define Q_OS_UNIX
 #elif defined(__MAKEDEPEND__)
 #else
 #  error "Qt has not been ported to this OS - see http://www.qt-project.org/"
