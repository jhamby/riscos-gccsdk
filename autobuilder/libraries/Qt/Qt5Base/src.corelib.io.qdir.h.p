--- src/corelib/io/qdir.h.orig	2020-10-27 01:02:11.000000000 -0700
+++ src/corelib/io/qdir.h	2021-08-22 19:16:56.999374097 -0700
@@ -190,7 +190,7 @@
 
     Q_DECL_CONSTEXPR static inline QChar listSeparator() noexcept
     {
-#if defined(Q_OS_WIN)
+#if defined(Q_OS_WIN) || defined(Q_OS_RISCOS)
         return QLatin1Char(';');
 #else
         return QLatin1Char(':');
