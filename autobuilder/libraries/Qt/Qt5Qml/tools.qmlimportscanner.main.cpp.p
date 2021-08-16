--- tools/qmlimportscanner/main.cpp.orig	2021-08-14 23:48:26.593394336 -0700
+++ tools/qmlimportscanner/main.cpp	2021-08-14 23:49:34.461705803 -0700
@@ -62,6 +62,16 @@
 
 static void printUsage(const QString &appNameIn)
 {
+#ifdef Q_OS_RISCOS
+    const std::string appName = appNameIn.toStdString();
+    const QString qmlPath = QLibraryInfo::location(QLibraryInfo::Qml2ImportsPath);
+    std::cerr
+        << "Usage: " << appName << " -rootPath path/to/app/qml/directory -importPath path/to/qt/qml/directory\n"
+           "       " << appName << " -qmlFiles file1 file2 -importPath path/to/qt/qml/directory\n\n"
+           "Example: " << appName << " -rootPath . -importPath "
+        << QDir::toNativeSeparators(qmlPath).toStdString()
+        << '\n';
+#else
     const std::wstring appName = appNameIn.toStdWString();
 #ifndef QT_BOOTSTRAPPED
     const QString qmlPath = QLibraryInfo::location(QLibraryInfo::Qml2ImportsPath);
@@ -74,6 +84,7 @@
            "Example: " << appName << " -rootPath . -importPath "
         << QDir::toNativeSeparators(qmlPath).toStdWString()
         << '\n';
+#endif
 }
 
 QVariantList findImportsInAst(QQmlJS::AST::UiHeaderItemList *headerItemList, const QString &code, const QString &path)
