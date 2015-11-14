--- src/gui/text/qcssparser_p.h.orig	2015-06-29 21:04:51.000000000 +0100
+++ src/gui/text/qcssparser_p.h	2015-07-21 11:45:43.478261538 +0100
@@ -62,7 +62,8 @@
 #ifndef QT_NO_CSSPARSER
 
 // VxWorks defines NONE as (-1) "for times when NULL won't do"
-#if defined(Q_OS_VXWORKS) && defined(NONE)
+// RISC OS: OSLib defines NONE as 0u
+#if (defined(Q_OS_VXWORKS) || defined(Q_OS_RISCOS)) && defined(NONE)
 #  undef NONE
 #endif
 #if defined(Q_OS_INTEGRITY)
