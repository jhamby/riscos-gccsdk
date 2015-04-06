--- src/corelib/global/qglobal.cpp.orig	2014-05-15 18:12:17.000000000 +0100
+++ src/corelib/global/qglobal.cpp	2014-05-27 19:55:16.374522842 +0100
@@ -2448,7 +2448,8 @@
 */
 void qsrand(uint seed)
 {
-#if defined(Q_OS_UNIX) && !defined(QT_NO_THREAD) && defined(_POSIX_THREAD_SAFE_FUNCTIONS) && (_POSIX_THREAD_SAFE_FUNCTIONS - 0 > 0)
+#if defined(Q_OS_UNIX) && !defined(QT_NO_THREAD) && defined(_POSIX_THREAD_SAFE_FUNCTIONS) && (_POSIX_THREAD_SAFE_FUNCTIONS - 0 > 0) \
+      && !defined(Q_OS_RISCOS)
     SeedStorage *seedStorage = randTLS();
     if (seedStorage) {
         SeedStorageType *pseed = seedStorage->localData();
@@ -2502,7 +2503,8 @@
 */
 int qrand()
 {
-#if defined(Q_OS_UNIX) && !defined(QT_NO_THREAD) && defined(_POSIX_THREAD_SAFE_FUNCTIONS) && (_POSIX_THREAD_SAFE_FUNCTIONS - 0 > 0)
+#if defined(Q_OS_UNIX) && !defined(QT_NO_THREAD) && defined(_POSIX_THREAD_SAFE_FUNCTIONS) && (_POSIX_THREAD_SAFE_FUNCTIONS - 0 > 0) \
+      && !defined(Q_OS_RISCOS)
     SeedStorage *seedStorage = randTLS();
     if (seedStorage) {
         SeedStorageType *pseed = seedStorage->localData();
