--- src/core/NetworkProxyFactory.cpp.orig	2016-01-29 11:49:14.610951107 +0000
+++ src/core/NetworkProxyFactory.cpp	2016-01-29 11:53:17.974951356 +0000
@@ -31,7 +31,7 @@
 {
 
 NetworkProxyFactory::NetworkProxyFactory() : QObject(), QNetworkProxyFactory(),
-	m_automaticProxy(NULL),
+//	m_automaticProxy(NULL),
 	m_pacNetworkReply(NULL),
 	m_proxyMode(SystemProxy)
 {
@@ -42,10 +42,12 @@
 
 NetworkProxyFactory::~NetworkProxyFactory()
 {
+#if !defined(Q_OS_RISCOS)
 	if (m_automaticProxy)
 	{
 		delete m_automaticProxy;
 	}
+#endif
 }
 
 void NetworkProxyFactory::optionChanged(const QString &option)
@@ -50,6 +50,7 @@
 
 void NetworkProxyFactory::optionChanged(const QString &option)
 {
+#if !defined(Q_OS_RISCOS)
 	if ((option == QLatin1String("Network/ProxyMode") && SettingsManager::getValue(option) == QLatin1String("automatic")) || (option == QLatin1String("Proxy/AutomaticConfigurationPath") && m_proxyMode == AutomaticProxy))
 	{
 		m_proxyMode = AutomaticProxy;
@@ -95,7 +96,9 @@
 			}
 		}
 	}
-	else if ((option == QLatin1String("Network/ProxyMode") && SettingsManager::getValue(option) == QLatin1String("manual")) || (option.startsWith(QLatin1String("Proxy/")) && m_proxyMode == ManualProxy))
+	else
+#endif
+        if ((option == QLatin1String("Network/ProxyMode") && SettingsManager::getValue(option) == QLatin1String("manual")) || (option.startsWith(QLatin1String("Proxy/")) && m_proxyMode == ManualProxy))
 	{
 		m_proxyMode = ManualProxy;
 
@@ -139,6 +142,7 @@
 	}
 }
 
+#if !defined(Q_OS_RISCOS) 
 void NetworkProxyFactory::setupAutomaticProxy()
 {
 	if (m_pacNetworkReply->error() != QNetworkReply::NoError || !m_automaticProxy->setup(m_pacNetworkReply->readAll()))
@@ -150,6 +154,7 @@
 
 	m_pacNetworkReply->deleteLater();
 }
+#endif
 
 QList<QNetworkProxy> NetworkProxyFactory::queryProxy(const QNetworkProxyQuery &query)
 {
@@ -203,12 +208,12 @@
 			return m_proxies[QLatin1String("NoProxy")];
 		}
 	}
-
+#if !defined(Q_OS_RISCOS)
 	if (m_proxyMode == AutomaticProxy && m_automaticProxy)
 	{
 		return m_automaticProxy->getProxy(query.url().toString(), query.peerHostName());
 	}
-
+#endif
 	return m_proxies[QLatin1String("NoProxy")];
 }
 
