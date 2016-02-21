--- src/lib/network/networkproxyfactory.cpp.orig	2016-02-21 11:46:41.686837196 +0000
+++ src/lib/network/networkproxyfactory.cpp	2016-02-21 11:48:55.738837333 +0000
@@ -18,7 +18,10 @@
 #include "networkproxyfactory.h"
 #include "mainapplication.h"
 #include "settings.h"
+
+#ifndef __riscos__
 #include "pac/pacmanager.h"
+#endif
 
 WildcardMatcher::WildcardMatcher(const QString &pattern)
     : m_regExp(0)
@@ -62,7 +65,9 @@
 
 NetworkProxyFactory::NetworkProxyFactory()
     : QNetworkProxyFactory()
+#ifndef __riscos__
     , m_pacManager(new PacManager)
+#endif
     , m_proxyPreference(SystemProxy)
     , m_proxyType(QNetworkProxy::HttpProxy)
     , m_port(0)
@@ -98,14 +103,17 @@
     foreach (const QString &exception, exceptions) {
         m_proxyExceptions.append(new WildcardMatcher(exception.trimmed()));
     }
-
+#ifndef __riscos__
     m_pacManager->loadSettings();
+#endif
 }
 
+#ifndef __riscos__
 PacManager* NetworkProxyFactory::pacManager() const
 {
     return m_pacManager;
 }
+#endif
 
 NetworkProxyFactory::ProxyPreference NetworkProxyFactory::proxyPreference() const
 {
@@ -135,7 +143,9 @@
         break;
 
     case ProxyAutoConfig:
+#ifndef __riscos__
         proxyList.append(m_pacManager->queryProxy(query.url()));
+#endif
         break;
 
     case DefinedProxy: {
