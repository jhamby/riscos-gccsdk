--- src/corelib/io/qstandardpaths_unix.cpp.orig	2021-08-22 20:41:04.976076179 -0700
+++ src/corelib/io/qstandardpaths_unix.cpp	2021-08-22 20:43:57.161322171 -0700
@@ -212,9 +212,9 @@
         // http://standards.freedesktop.org/basedir-spec/basedir-spec-0.6.html
         QString xdgCacheHome = QFile::decodeName(qgetenv("XDG_CACHE_HOME"));
         if (isTestModeEnabled())
-            xdgCacheHome = QDir::homePath() + QLatin1String("/.qttest/cache");
+            xdgCacheHome = QLatin1String("/<Choices$Write>/Qt5/qttest/cache");
         if (xdgCacheHome.isEmpty())
-            xdgCacheHome = QDir::homePath() + QLatin1String("/.cache");
+            xdgCacheHome = QLatin1String("/<Choices$Write>/Qt5/cache");
         if (type == QStandardPaths::CacheLocation)
             appendOrganizationAndApp(xdgCacheHome);
         return xdgCacheHome;
@@ -225,9 +225,9 @@
     {
         QString xdgDataHome = QFile::decodeName(qgetenv("XDG_DATA_HOME"));
         if (isTestModeEnabled())
-            xdgDataHome = QDir::homePath() + QLatin1String("/.qttest/share");
+            xdgDataHome = QLatin1String("/<Choices$Write>/Qt5/qttest/share");
         if (xdgDataHome.isEmpty())
-            xdgDataHome = QDir::homePath() + QLatin1String("/.local/share");
+            xdgDataHome = QLatin1String("/<Choices$Write>/Qt5/local/share");
         if (type == AppDataLocation || type == AppLocalDataLocation)
             appendOrganizationAndApp(xdgDataHome);
         return xdgDataHome;
@@ -239,9 +239,9 @@
         // http://standards.freedesktop.org/basedir-spec/latest/
         QString xdgConfigHome = QFile::decodeName(qgetenv("XDG_CONFIG_HOME"));
         if (isTestModeEnabled())
-            xdgConfigHome = QDir::homePath() + QLatin1String("/.qttest/config");
+            xdgConfigHome = QLatin1String("/<Choices$Write>/Qt5/qttest/config");
         if (xdgConfigHome.isEmpty())
-            xdgConfigHome = QDir::homePath() + QLatin1String("/.config");
+            xdgConfigHome = QLatin1String("/<Choices$Write>/Qt5/config");
         if (type == AppConfigLocation)
             appendOrganizationAndApp(xdgConfigHome);
         return xdgConfigHome;
@@ -276,7 +276,7 @@
     // http://www.freedesktop.org/wiki/Software/xdg-user-dirs
     QString xdgConfigHome = QFile::decodeName(qgetenv("XDG_CONFIG_HOME"));
     if (xdgConfigHome.isEmpty())
-        xdgConfigHome = QDir::homePath() + QLatin1String("/.config");
+        xdgConfigHome = QLatin1String("/<Choices$Write>/Qt5/config");
     QFile file(xdgConfigHome + QLatin1String("/user-dirs.dirs"));
     const QLatin1String key = xdg_key_name(type);
     if (!key.isEmpty() && !isTestModeEnabled() && file.open(QIODevice::ReadOnly)) {
@@ -350,10 +350,10 @@
     // http://standards.freedesktop.org/basedir-spec/latest/
     QString xdgDataDirsEnv = QFile::decodeName(qgetenv("XDG_DATA_DIRS"));
     if (xdgDataDirsEnv.isEmpty()) {
-        dirs.append(QString::fromLatin1("/usr/local/share"));
-        dirs.append(QString::fromLatin1("/usr/share"));
+        dirs.append(QString::fromLatin1("/<Choices$Write>/Qt5/local/share"));
+        dirs.append(QString::fromLatin1("/<Choices$Write>/Qt5/share"));
     } else {
-        const auto parts = xdgDataDirsEnv.splitRef(QLatin1Char(':'), Qt::SkipEmptyParts);
+        const auto parts = xdgDataDirsEnv.splitRef(QDir::listSeparator(), Qt::SkipEmptyParts);
 
         // Normalize paths, skip relative paths
         for (const QStringRef &dir : parts) {
@@ -378,9 +378,9 @@
     // http://standards.freedesktop.org/basedir-spec/latest/
     const QString xdgConfigDirs = QFile::decodeName(qgetenv("XDG_CONFIG_DIRS"));
     if (xdgConfigDirs.isEmpty())
-        dirs.append(QString::fromLatin1("/etc/xdg"));
+            dirs.append(QString::fromLatin1("/<Choices$Write>/Qt5/etc/xdg"));
     else
-        dirs = xdgConfigDirs.split(QLatin1Char(':'));
+        dirs = xdgConfigDirs.split(QDir::listSeparator());
     return dirs;
 }
 
