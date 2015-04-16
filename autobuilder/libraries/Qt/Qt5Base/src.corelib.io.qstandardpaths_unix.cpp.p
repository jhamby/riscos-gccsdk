--- src/corelib/io/qstandardpaths_unix.cpp.orig	2015-02-17 04:56:51.000000000 +0000
+++ src/corelib/io/qstandardpaths_unix.cpp	2015-04-12 17:44:12.341070591 +0100
@@ -75,9 +75,9 @@
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
@@ -88,9 +88,9 @@
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
@@ -101,9 +101,9 @@
         // http://standards.freedesktop.org/basedir-spec/latest/
         QString xdgConfigHome = QFile::decodeName(qgetenv("XDG_CONFIG_HOME"));
         if (isTestModeEnabled())
-            xdgConfigHome = QDir::homePath() + QLatin1String("/.qttest/config");
+            xdgConfigHome = QLatin1String("/<Choices$Write>/Qt5/qttest/config");
         if (xdgConfigHome.isEmpty())
-            xdgConfigHome = QDir::homePath() + QLatin1String("/.config");
+            xdgConfigHome = QLatin1String("/<Choices$Write>/Qt5/config");
         return xdgConfigHome;
     }
     case RuntimeLocation:
@@ -150,7 +150,7 @@
     // http://www.freedesktop.org/wiki/Software/xdg-user-dirs
     QString xdgConfigHome = QFile::decodeName(qgetenv("XDG_CONFIG_HOME"));
     if (xdgConfigHome.isEmpty())
-        xdgConfigHome = QDir::homePath() + QLatin1String("/.config");
+        xdgConfigHome = QLatin1String("/<Choices$Write>/Qt5/config");
     QFile file(xdgConfigHome + QLatin1String("/user-dirs.dirs"));
     if (!isTestModeEnabled() && file.open(QIODevice::ReadOnly)) {
         QHash<QString, QString> lines;
@@ -252,8 +252,8 @@
     // http://standards.freedesktop.org/basedir-spec/latest/
     QString xdgDataDirsEnv = QFile::decodeName(qgetenv("XDG_DATA_DIRS"));
     if (xdgDataDirsEnv.isEmpty()) {
-        dirs.append(QString::fromLatin1("/usr/local/share"));
-        dirs.append(QString::fromLatin1("/usr/share"));
+        dirs.append(QString::fromLatin1("/<Choices$Write>/Qt5/local/share"));
+        dirs.append(QString::fromLatin1("/<Choices$Write>/Qt5/share"));
     } else {
         dirs = xdgDataDirsEnv.split(QLatin1Char(':'), QString::SkipEmptyParts);
 
@@ -288,7 +288,7 @@
         // http://standards.freedesktop.org/basedir-spec/latest/
         const QString xdgConfigDirs = QFile::decodeName(qgetenv("XDG_CONFIG_DIRS"));
         if (xdgConfigDirs.isEmpty())
-            dirs.append(QString::fromLatin1("/etc/xdg"));
+            dirs.append(QString::fromLatin1("/<Choices$Write>/Qt5/etc/xdg"));
         else
             dirs = xdgConfigDirs.split(QLatin1Char(':'));
     }
