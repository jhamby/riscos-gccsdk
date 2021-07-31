--- src/lib/network/networkproxyfactory.h.orig	2016-02-21 11:36:57.610836599 +0000
+++ src/lib/network/networkproxyfactory.h	2016-02-21 11:38:16.978836681 +0000
@@ -24,7 +24,9 @@
 #include "qzcommon.h"
 #include "qzregexp.h"
 
+#ifndef __riscos__
 class PacManager;
+#endif
 
 class WildcardMatcher
 {
@@ -52,13 +54,17 @@
 
     void loadSettings();
 
+#ifndef __riscos__
     PacManager* pacManager() const;
+#endif
     ProxyPreference proxyPreference() const;
 
     QList<QNetworkProxy> queryProxy(const QNetworkProxyQuery &query = QNetworkProxyQuery());
 
 private:
+#ifndef __riscos__
     PacManager* m_pacManager;
+#endif
 
     ProxyPreference m_proxyPreference;
     QNetworkProxy::ProxyType m_proxyType;
