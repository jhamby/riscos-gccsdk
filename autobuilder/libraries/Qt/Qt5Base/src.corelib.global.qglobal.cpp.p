--- src/corelib/global/qglobal.cpp.orig	2015-02-17 04:56:50.000000000 +0000
+++ src/corelib/global/qglobal.cpp	2015-05-08 20:06:12.264661240 +0100
@@ -2676,6 +2676,22 @@
     return unknownText();
 }
 
+#ifdef Q_OS_RISCOS
+bool QSysInfo::RedBlueSwapVarRead = false;
+bool QSysInfo::RequireRedBlueSwap;
+
+bool QSysInfo::requireRedBlueSwap()
+{
+    if (!RedBlueSwapVarRead) {
+	QByteArray sysvar = qgetenv("Qt$NoRedBlueSwap");
+	RequireRedBlueSwap = sysvar.isEmpty();
+	RedBlueSwapVarRead = true;
+    }
+
+    return RequireRedBlueSwap;
+}
+#endif
+
 /*!
     \macro void Q_ASSERT(bool test)
     \relates <QtGlobal>
@@ -3192,7 +3208,8 @@
 */
 void qsrand(uint seed)
 {
-#if defined(Q_OS_UNIX) && !defined(QT_NO_THREAD) && defined(_POSIX_THREAD_SAFE_FUNCTIONS) && (_POSIX_THREAD_SAFE_FUNCTIONS - 0 > 0)
+#if defined(Q_OS_UNIX) && !defined(QT_NO_THREAD) && defined(_POSIX_THREAD_SAFE_FUNCTIONS) && (_POSIX_THREAD_SAFE_FUNCTIONS - 0 > 0) \
+      && !defined(Q_OS_RISCOS)
     SeedStorage *seedStorage = randTLS();
     if (seedStorage) {
         SeedStorageType *pseed = seedStorage->localData();
@@ -3246,7 +3263,8 @@
 */
 int qrand()
 {
-#if defined(Q_OS_UNIX) && !defined(QT_NO_THREAD) && defined(_POSIX_THREAD_SAFE_FUNCTIONS) && (_POSIX_THREAD_SAFE_FUNCTIONS - 0 > 0)
+#if defined(Q_OS_UNIX) && !defined(QT_NO_THREAD) && defined(_POSIX_THREAD_SAFE_FUNCTIONS) && (_POSIX_THREAD_SAFE_FUNCTIONS - 0 > 0) \
+      && !defined(Q_OS_RISCOS)
     SeedStorage *seedStorage = randTLS();
     if (seedStorage) {
         SeedStorageType *pseed = seedStorage->localData();
