--- src/corelib/global/qsystemdetection.h.orig	2015-07-15 18:24:19.842735895 +0100
+++ src/corelib/global/qsystemdetection.h	2015-07-15 18:42:38.078750127 +0100
@@ -172,6 +172,9 @@
 #  define Q_OS_VXWORKS
 #elif defined(__HAIKU__)
 #  define Q_OS_HAIKU
+#elif defined(__riscos__)
+#  define Q_OS_RISCOS
+#  define Q_OS_UNIX
 #elif defined(__MAKEDEPEND__)
 #else
 #  error "Qt has not been ported to this OS - see http://www.qt-project.org/"
