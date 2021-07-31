--- src/core/NetworkCache.cpp
+++ src/core/NetworkCache.cpp
@@ -38,7 +38,7 @@ NetworkCache::NetworkCache(QObject *parent) : QNetworkDiskCache(parent)
 		setCacheDirectory(cachePath);
 		setMaximumCacheSize(SettingsManager::getOption(SettingsManager::Cache_DiskCacheLimitOption).toInt() * 1024);
 
-		connect(SettingsManager::getInstance(), &SettingsManager::optionChanged, this, &NetworkCache::handleOptionChanged);
+		connect(SettingsManager::getInstance(), SIGNAL(optionChanged(int,QVariant)), this, SLOT(handleOptionChanged(int,QVariant)));
 	}
 }
 


