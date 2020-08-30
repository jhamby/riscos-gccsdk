--- src/core/NetworkProxyFactory.h
+++ src/core/NetworkProxyFactory.h
@@ -46,7 +46,9 @@ protected:
 	QNetworkProxy::ProxyType getProxyType(ProxyDefinition::ProtocolType protocol);
 
 private:
+#if !defined(Q_OS_RISCOS)
 	NetworkAutomaticProxy *m_automaticProxy;
+#endif
 	ProxyDefinition m_definition;
 	QMap<int, QList<QNetworkProxy> > m_proxies;
 };


