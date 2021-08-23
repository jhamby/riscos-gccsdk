--- src/corelib/io/qdir.cpp.orig	2020-10-27 01:02:11.000000000 -0700
+++ src/corelib/io/qdir.cpp	2021-08-22 19:21:07.292281708 -0700
@@ -759,8 +759,11 @@
     // a colon in the path.
     // FIXME: relies on virtual file-systems having colons in their prefixes.
     // The case of an MS-absolute C:/... path happens to work either way.
-    return (path.contains(QLatin1Char(':')) && QFileInfo(path).isAbsolute())
-        || QFileSystemEntry(path).isAbsolute();
+    return
+#if !defined(Q_OS_RISCOS)
+        (path.contains(QLatin1Char(':')) && QFileInfo(path).isAbsolute()) ||
+#endif
+        QFileSystemEntry(path).isAbsolute();
 }
 
 /*!
