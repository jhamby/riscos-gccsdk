--- src/core/NetworkProxyFactory.cpp.orig	2017-02-15 20:46:40.290480756 +0000
+++ src/core/NetworkProxyFactory.cpp	2017-02-15 20:54:58.205539176 +0000
@@ -19,22 +19,25 @@
 **************************************************************************/
 
 #include "NetworkProxyFactory.h"
+#if !defined(Q_OS_RISCOS)
 #include "NetworkAutomaticProxy.h"
+#endif
 
 namespace Otter
 {
 
-NetworkProxyFactory::NetworkProxyFactory(QObject *parent) : QObject(parent), QNetworkProxyFactory(),
-	m_automaticProxy(nullptr)
+NetworkProxyFactory::NetworkProxyFactory(QObject *parent) : QObject(parent), QNetworkProxyFactory()
 {
 }
 
 NetworkProxyFactory::~NetworkProxyFactory()
 {
+#if !defined(Q_OS_RISCOS)
 	if (m_automaticProxy)
 	{
 		delete m_automaticProxy;
 	}
+#endif
 }
 
 void NetworkProxyFactory::setProxy(const QString &identifier)
@@ -69,6 +72,7 @@
 			}
 
 			break;
+#if !defined(Q_OS_RISCOS)
 		case ProxyDefinition::AutomaticProxy:
 			if (m_automaticProxy)
 			{
@@ -80,6 +84,7 @@
 			}
 
 			break;
+#endif
 		default:
 			break;
 	}
@@ -140,6 +145,7 @@
 			}
 
 			break;
+#if !defined(Q_OS_RISCOS)
 		case ProxyDefinition::AutomaticProxy:
 			if (m_automaticProxy && m_automaticProxy->isValid())
 			{
@@ -149,6 +155,7 @@
 			return QNetworkProxyFactory::systemProxyForQuery(query);
 		default:
 			break;
+#endif
 	}
 
 	return m_proxies[-1];
