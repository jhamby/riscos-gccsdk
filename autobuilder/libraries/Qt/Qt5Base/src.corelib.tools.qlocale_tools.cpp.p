--- src/corelib/tools/qlocale_tools.cpp.orig	2015-06-29 21:05:05.000000000 +0100
+++ src/corelib/tools/qlocale_tools.cpp	2015-07-15 18:24:20.394735902 +0100
@@ -1982,7 +1982,12 @@
     feholdexcept(&envp);
 #endif
 
+#ifdef Q_OS_RISCOS
+    char *s = __dtoa (d, mode, ndigits, decpt, sign, rve);
+    *resultp = s;
+#else
     char *s = _qdtoa(d, mode, ndigits, decpt, sign, rve, resultp);
+#endif
 
 #ifdef Q_OS_WIN
     _clear87();
