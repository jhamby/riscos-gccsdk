--- src/core/NetworkProxyFactory.h.orig	2016-09-07 08:13:14.737610559 +0100
+++ src/core/NetworkProxyFactory.h	2016-09-07 08:31:27.729611677 +0100
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
 	void optionChanged(int identifier);
-	void setupAutomaticProxy();
 
 private:
-	NetworkAutomaticProxy *m_automaticProxy;
 	QNetworkReply *m_pacNetworkReply;
 	QStringList m_proxyExceptions;
 	QHash<QString, QList<QNetworkProxy> > m_proxies;
