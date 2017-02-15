--- src/core/NetworkProxyFactory.h.orig	2017-02-15 20:46:40.382479263 +0000
+++ src/core/NetworkProxyFactory.h	2017-02-15 20:48:06.817341536 +0000
@@ -46,7 +46,9 @@
 	QNetworkProxy::ProxyType getProxyType(ProxyDefinition::ProtocolType protocol);
 
 private:
+#if !defined(Q_OS_RISCOS)
 	NetworkAutomaticProxy *m_automaticProxy;
+#endif
 	ProxyDefinition m_definition;
 	QMap<int, QList<QNetworkProxy> > m_proxies;
 };
