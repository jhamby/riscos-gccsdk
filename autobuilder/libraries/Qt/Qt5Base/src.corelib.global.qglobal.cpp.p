--- src/corelib/global/qglobal.cpp.orig	2021-08-14 18:15:57.599582670 -0700
+++ src/corelib/global/qglobal.cpp	2021-08-14 18:16:41.027790774 -0700
@@ -3340,7 +3340,8 @@
 */
 void qsrand(uint seed)
 {
-#if defined(Q_OS_UNIX) && !defined(QT_NO_THREAD) && defined(_POSIX_THREAD_SAFE_FUNCTIONS) && (_POSIX_THREAD_SAFE_FUNCTIONS - 0 > 0)
+#if defined(Q_OS_UNIX) && !defined(QT_NO_THREAD) && defined(_POSIX_THREAD_SAFE_FUNCTIONS) && (_POSIX_THREAD_SAFE_FUNCTIONS - 0 > 0) \
+      && !defined(Q_OS_RISCOS)
     SeedStorage *seedStorage = randTLS();
     if (seedStorage) {
         SeedStorageType *pseed = seedStorage->localData();
@@ -3394,7 +3395,8 @@
 */
 int qrand()
 {
-#if defined(Q_OS_UNIX) && !defined(QT_NO_THREAD) && defined(_POSIX_THREAD_SAFE_FUNCTIONS) && (_POSIX_THREAD_SAFE_FUNCTIONS - 0 > 0)
+#if defined(Q_OS_UNIX) && !defined(QT_NO_THREAD) && defined(_POSIX_THREAD_SAFE_FUNCTIONS) && (_POSIX_THREAD_SAFE_FUNCTIONS - 0 > 0) \
+      && !defined(Q_OS_RISCOS)
     SeedStorage *seedStorage = randTLS();
     if (seedStorage) {
         SeedStorageType *pseed = seedStorage->localData();
