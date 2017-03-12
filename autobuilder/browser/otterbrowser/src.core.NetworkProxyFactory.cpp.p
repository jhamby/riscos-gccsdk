--- src/core/NetworkProxyFactory.cpp.orig	2017-03-12 08:09:05.047277420 +0000
+++ src/core/NetworkProxyFactory.cpp	2017-03-12 08:15:29.750000000 +0000
@@ -19,23 +19,29 @@
 **************************************************************************/
 
 #include "NetworkProxyFactory.h"
+#if !defined(Q_OS_RISCOS)
 #include "NetworkAutomaticProxy.h"
+#endif
 
 namespace Otter
 {
 
 NetworkProxyFactory::NetworkProxyFactory(QObject *parent) : QObject(parent), QNetworkProxyFactory(),
+#if !defined(Q_OS_RISCOS)
 	m_automaticProxy(nullptr),
+#endif
 	m_definition(ProxyDefinition())
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
@@ -70,6 +76,7 @@
 			}
 
 			break;
+#if !defined(Q_OS_RISCOS)
 		case ProxyDefinition::AutomaticProxy:
 			if (m_automaticProxy)
 			{
@@ -81,6 +88,7 @@
 			}
 
 			break;
+#endif
 		default:
 			break;
 	}
@@ -141,6 +149,7 @@
 			}
 
 			break;
+#if !defined(Q_OS_RISCOS)
 		case ProxyDefinition::AutomaticProxy:
 			if (m_automaticProxy && m_automaticProxy->isValid())
 			{
@@ -150,6 +159,7 @@
 			return QNetworkProxyFactory::systemProxyForQuery(query);
 		default:
 			break;
+#endif
 	}
 
 	return m_proxies[-1];
