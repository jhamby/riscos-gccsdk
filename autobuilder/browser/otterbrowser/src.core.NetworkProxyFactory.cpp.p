--- src/core/NetworkProxyFactory.cpp.orig	2016-11-12 08:32:19.850039614 +0000
+++ src/core/NetworkProxyFactory.cpp	2016-11-12 08:33:29.910039685 +0000
@@ -31,7 +31,7 @@
 {
 
 NetworkProxyFactory::NetworkProxyFactory() : QObject(), QNetworkProxyFactory(),
-	m_automaticProxy(nullptr),
+//	m_automaticProxy(nullptr),
 	m_pacNetworkReply(nullptr),
 	m_proxyMode(SystemProxy)
 {
@@ -42,14 +42,17 @@
 
 NetworkProxyFactory::~NetworkProxyFactory()
 {
+#if !defined(Q_OS_RISCOS)
 	if (m_automaticProxy)
 	{
 		delete m_automaticProxy;
 	}
+#endif
 }
 
 void NetworkProxyFactory::optionChanged(int identifier)
 {
+#if !defined(Q_OS_RISCOS)
 	if ((identifier == SettingsManager::Network_ProxyModeOption && SettingsManager::getValue(identifier) == QLatin1String("automatic")) || (identifier == SettingsManager::Proxy_AutomaticConfigurationPathOption && m_proxyMode == AutomaticProxy))
 	{
 		m_proxyMode = AutomaticProxy;
@@ -95,7 +98,9 @@
 			}
 		}
 	}
-	else if ((identifier == SettingsManager::Network_ProxyModeOption && SettingsManager::getValue(identifier) == QLatin1String("manual")) || (SettingsManager::getOptionName(identifier).startsWith(QLatin1String("Proxy/")) && m_proxyMode == ManualProxy))
+	else
+#endif
+        if ((identifier == SettingsManager::Network_ProxyModeOption && SettingsManager::getValue(identifier) == QLatin1String("manual")) || (SettingsManager::getOptionName(identifier).startsWith(QLatin1String("Proxy/")) && m_proxyMode == ManualProxy))
 	{
 		m_proxyMode = ManualProxy;
 
@@ -138,6 +143,7 @@
 	}
 }
 
+#if !defined(Q_OS_RISCOS) 
 void NetworkProxyFactory::setupAutomaticProxy()
 {
 	if (m_pacNetworkReply->error() != QNetworkReply::NoError || !m_automaticProxy->setup(m_pacNetworkReply->readAll()))
@@ -149,6 +155,7 @@
 
 	m_pacNetworkReply->deleteLater();
 }
+#endif
 
 QList<QNetworkProxy> NetworkProxyFactory::queryProxy(const QNetworkProxyQuery &query)
 {
@@ -203,12 +210,12 @@
 
 		return m_proxies[QLatin1String("NoProxy")];
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
 
