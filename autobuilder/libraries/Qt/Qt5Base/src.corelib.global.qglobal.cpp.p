--- src/corelib/global/qglobal.cpp.orig	2021-07-31 02:14:34.798323992 -0700
+++ src/corelib/global/qglobal.cpp	2021-07-31 02:14:50.590452580 -0700
@@ -2774,6 +2774,22 @@
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
@@ -3340,7 +3356,8 @@
 */
 void qsrand(uint seed)
 {
-#if defined(Q_OS_UNIX) && !defined(QT_NO_THREAD) && defined(_POSIX_THREAD_SAFE_FUNCTIONS) && (_POSIX_THREAD_SAFE_FUNCTIONS - 0 > 0)
+#if defined(Q_OS_UNIX) && !defined(QT_NO_THREAD) && defined(_POSIX_THREAD_SAFE_FUNCTIONS) && (_POSIX_THREAD_SAFE_FUNCTIONS - 0 > 0) \
+      && !defined(Q_OS_RISCOS)
     SeedStorage *seedStorage = randTLS();
     if (seedStorage) {
         SeedStorageType *pseed = seedStorage->localData();
@@ -3394,7 +3411,8 @@
 */
 int qrand()
 {
-#if defined(Q_OS_UNIX) && !defined(QT_NO_THREAD) && defined(_POSIX_THREAD_SAFE_FUNCTIONS) && (_POSIX_THREAD_SAFE_FUNCTIONS - 0 > 0)
+#if defined(Q_OS_UNIX) && !defined(QT_NO_THREAD) && defined(_POSIX_THREAD_SAFE_FUNCTIONS) && (_POSIX_THREAD_SAFE_FUNCTIONS - 0 > 0) \
+      && !defined(Q_OS_RISCOS)
     SeedStorage *seedStorage = randTLS();
     if (seedStorage) {
         SeedStorageType *pseed = seedStorage->localData();
