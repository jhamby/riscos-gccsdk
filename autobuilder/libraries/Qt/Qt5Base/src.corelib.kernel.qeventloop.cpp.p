--- src/corelib/kernel/qeventloop.cpp.orig	2014-05-15 18:12:17.000000000 +0100
+++ src/corelib/kernel/qeventloop.cpp	2015-03-31 17:48:13.874083237 +0100
@@ -133,7 +133,12 @@
     Q_D(QEventLoop);
     if (!d->threadData->eventDispatcher.load())
         return false;
+#ifdef __riscos__
+    bool result = d->threadData->eventDispatcher.load()->processEvents(flags);
+    pthread_yield();
+#else
     return d->threadData->eventDispatcher.load()->processEvents(flags);
+#endif
 }
 
 /*!
