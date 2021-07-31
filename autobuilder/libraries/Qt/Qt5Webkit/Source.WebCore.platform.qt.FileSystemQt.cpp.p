--- Source/WebCore/platform/qt/FileSystemQt.cpp.orig	2014-05-15 18:12:29.000000000 +0100
+++ Source/WebCore/platform/qt/FileSystemQt.cpp	2014-06-17 18:50:43.298663019 +0100
@@ -44,6 +44,8 @@
 
 #if OS(ANDROID)
 #include <sys/vfs.h>
+#elif OS(RISCOS)
+#include <sys/statfs.h>
 #elif !defined(Q_OS_WIN)
 #include <sys/statvfs.h>
 #endif
@@ -216,6 +218,10 @@
     if (!result)
         return 0;
     return static_cast<uint64_t>(freeBytesToCaller.QuadPart);
+#elif OS(RISCOS)
+    printf ("[%s:%d:%s] - not implemented; returning 0.\n",
+	    __func__, __LINE__, __FILE__);
+    return 0;
 #else
 #if OS(ANDROID)
     struct statfs volumeInfo;
