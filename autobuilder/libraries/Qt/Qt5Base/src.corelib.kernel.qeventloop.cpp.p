--- src/corelib/kernel/qeventloop.cpp.orig	2015-02-17 04:56:50.000000000 +0000
+++ src/corelib/kernel/qeventloop.cpp	2015-04-12 19:57:16.965299576 +0100
@@ -125,7 +125,13 @@
     Q_D(QEventLoop);
     if (!d->threadData->eventDispatcher.load())
         return false;
+#ifdef __riscos__
+    bool result = d->threadData->eventDispatcher.load()->processEvents(flags);
+    pthread_yield();
+    return result;
+#else
     return d->threadData->eventDispatcher.load()->processEvents(flags);
+#endif
 }
 
 /*!
