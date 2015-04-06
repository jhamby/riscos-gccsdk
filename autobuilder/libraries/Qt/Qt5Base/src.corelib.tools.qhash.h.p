--- src/corelib/tools/qhash.h.orig	2014-05-15 18:12:16.000000000 +0100
+++ src/corelib/tools/qhash.h	2014-05-27 20:28:23.798548596 +0100
@@ -96,7 +96,7 @@
 #ifndef Q_OS_DARWIN
 Q_CORE_EXPORT uint qHash(long double key, uint seed = 0) Q_DECL_NOTHROW;
 #endif
-inline uint qHash(QChar key, uint seed = 0) Q_DECL_NOTHROW { return qHash(key.unicode(), seed); }
+inline uint qHash(QChar key, uint seed = 0) Q_DECL_NOTHROW { return qHash(RISCOS_DEREF_QCHAR(key.unicode()), seed); }
 Q_CORE_EXPORT uint qHash(const QByteArray &key, uint seed = 0) Q_DECL_NOTHROW;
 Q_CORE_EXPORT uint qHash(const QString &key, uint seed = 0) Q_DECL_NOTHROW;
 Q_CORE_EXPORT uint qHash(const QStringRef &key, uint seed = 0) Q_DECL_NOTHROW;
