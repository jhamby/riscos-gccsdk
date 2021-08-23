--- src/corelib/global/qlibraryinfo.cpp.orig	2020-10-27 01:02:11.000000000 -0700
+++ src/corelib/global/qlibraryinfo.cpp	2021-08-22 20:54:51.951576260 -0700
@@ -847,12 +847,16 @@
                 sysrootify(&baseDir);
         }
 #else
+#if defined(Q_OS_RISCOS)
+        baseDir = QString::fromLocal8Bit(QT_CONFIGURE_PREFIX_PATH);
+#else
         if (loc == PrefixPath) {
             baseDir = prefixFromAppDirHelper();
         } else {
             // we make any other path absolute to the prefix directory
             baseDir = location(PrefixPath);
         }
+#endif
 #endif // QT_BUILD_QMAKE
         ret = QDir::cleanPath(baseDir + QLatin1Char('/') + ret);
     }
