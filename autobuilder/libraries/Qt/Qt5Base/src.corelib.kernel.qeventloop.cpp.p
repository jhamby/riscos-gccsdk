--- ./src/corelib/kernel/qeventloop.cpp.orig	2021-08-22 12:02:10.625631527 -0700
+++ ./src/corelib/kernel/qeventloop.cpp	2021-08-22 14:48:25.387140660 -0700
@@ -136,7 +136,13 @@
     auto threadData = d->threadData.loadRelaxed();
     if (!threadData->hasEventDispatcher())
         return false;
+#ifdef __riscos__
+    bool result = threadData->eventDispatcher.loadRelaxed()->processEvents(flags);
+    pthread_yield();
+    return result;
+#else
     return threadData->eventDispatcher.loadRelaxed()->processEvents(flags);
+#endif
 }
 
 /*!
