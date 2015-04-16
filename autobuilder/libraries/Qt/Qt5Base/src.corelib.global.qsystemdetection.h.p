--- src/corelib/global/qsystemdetection.h.orig	2015-02-17 04:56:50.000000000 +0000
+++ src/corelib/global/qsystemdetection.h	2015-04-12 13:43:51.556657030 +0100
@@ -169,6 +169,9 @@
 #  define Q_OS_INTEGRITY
 #elif defined(VXWORKS) /* there is no "real" VxWorks define - this has to be set in the mkspec! */
 #  define Q_OS_VXWORKS
+#elif defined(__riscos__)
+#  define Q_OS_RISCOS
+#  define Q_OS_UNIX
 #elif defined(__MAKEDEPEND__)
 #else
 #  error "Qt has not been ported to this OS - see http://www.qt-project.org/"
