--- 3rdparty/sonnet/src/plugins/hunspell/hunspellclient.cpp
+++ 3rdparty/sonnet/src/plugins/hunspell/hunspellclient.cpp
@@ -56,6 +56,8 @@ HunspellClient::HunspellClient(QObject *parent)
 
 #ifdef Q_OS_MAC
     directories << QLatin1String("/System/Library/Spelling/");
+#elif defined Q_OS_RISCOS
+    directories << QLatin1String("/<Hunspell$Dir>/");
 #else
     directories << QLatin1String("/usr/share/hunspell/") << QLatin1String("/usr/local/share/hunspell/") << QLatin1String("/usr/share/myspell/") << QLatin1String("/usr/share/myspell/dicts/") << QLatin1String("/usr/local/share/mozilla-dicts/");
 #endif


