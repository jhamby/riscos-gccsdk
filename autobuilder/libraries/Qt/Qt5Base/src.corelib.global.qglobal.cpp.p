--- src/corelib/global/qglobal.cpp.orig	2015-06-29 21:04:58.000000000 +0100
+++ src/corelib/global/qglobal.cpp	2015-07-15 18:24:20.410735903 +0100
@@ -2778,6 +2778,22 @@
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
@@ -3344,7 +3360,8 @@
 */
 void qsrand(uint seed)
 {
-#if defined(Q_OS_UNIX) && !defined(QT_NO_THREAD) && defined(_POSIX_THREAD_SAFE_FUNCTIONS) && (_POSIX_THREAD_SAFE_FUNCTIONS - 0 > 0)
+#if defined(Q_OS_UNIX) && !defined(QT_NO_THREAD) && defined(_POSIX_THREAD_SAFE_FUNCTIONS) && (_POSIX_THREAD_SAFE_FUNCTIONS - 0 > 0) \
+      && !defined(Q_OS_RISCOS)
     SeedStorage *seedStorage = randTLS();
     if (seedStorage) {
         SeedStorageType *pseed = seedStorage->localData();
@@ -3398,7 +3415,8 @@
 */
 int qrand()
 {
-#if defined(Q_OS_UNIX) && !defined(QT_NO_THREAD) && defined(_POSIX_THREAD_SAFE_FUNCTIONS) && (_POSIX_THREAD_SAFE_FUNCTIONS - 0 > 0)
+#if defined(Q_OS_UNIX) && !defined(QT_NO_THREAD) && defined(_POSIX_THREAD_SAFE_FUNCTIONS) && (_POSIX_THREAD_SAFE_FUNCTIONS - 0 > 0) \
+      && !defined(Q_OS_RISCOS)
     SeedStorage *seedStorage = randTLS();
     if (seedStorage) {
         SeedStorageType *pseed = seedStorage->localData();
