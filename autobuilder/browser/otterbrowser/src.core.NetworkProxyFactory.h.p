--- src/core/NetworkProxyFactory.h.orig	2016-01-29 15:25:44.638728613 +0000
+++ src/core/NetworkProxyFactory.h	2016-01-29 15:26:16.846728646 +0000
@@ -21,8 +21,6 @@
 #ifndef OTTER_NETWORKPROXYFACTORY_H
 #define OTTER_NETWORKPROXYFACTORY_H
 
-#include "NetworkAutomaticProxy.h"
-
 #include <QtNetwork/QNetworkProxy>
 #include <QtNetwork/QNetworkReply>
 
@@ -42,18 +40,15 @@
 	{
 		NoProxy = 0,
 		ManualProxy = 1,
-		SystemProxy = 2,
-		AutomaticProxy = 3
+		SystemProxy = 2
 	};
 
 	QList<QNetworkProxy> queryProxy(const QNetworkProxyQuery &query);
 
 protected slots:
 	void optionChanged(const QString &option);
-	void setupAutomaticProxy();
 
 private:
-	NetworkAutomaticProxy *m_automaticProxy;
 	QNetworkReply *m_pacNetworkReply;
 	QStringList m_proxyExceptions;
 	QHash<QString, QList<QNetworkProxy> > m_proxies;
