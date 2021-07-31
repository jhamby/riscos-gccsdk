--- src/core/NetworkProxyFactory.cpp.orig	2020-08-30 18:02:59.653980593 +0100
+++ src/core/NetworkProxyFactory.cpp	2020-08-30 18:10:38.720733367 +0100
@@ -1,6 +1,6 @@
 /**************************************************************************
 * Otter Browser: Web browser controlled by the user, not vice-versa.
-* Copyright (C) 2013 - 2017 Michal Dutkiewicz aka Emdek <michal@emdek.pl>
+* Copyright (C) 2013 - 2018 Michal Dutkiewicz aka Emdek <michal@emdek.pl>
 * Copyright (C) 2014 - 2017 Jan Bajer aka bajasoft <jbajer@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
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
@@ -55,12 +61,13 @@
 				{
 					if (iterator.key() == ProxyDefinition::AnyProtocol)
 					{
-						const QList<ProxyDefinition::ProtocolType> protocols({ProxyDefinition::HttpProtocol, ProxyDefinition::HttpsProtocol, ProxyDefinition::FtpProtocol, ProxyDefinition::SocksProtocol});
+						const QVector<ProxyDefinition::ProtocolType> protocols({ProxyDefinition::HttpProtocol, ProxyDefinition::HttpsProtocol, ProxyDefinition::FtpProtocol});
 
 						for (int i = 0; i < protocols.count(); ++i)
 						{
-							m_proxies[iterator.key()] = QList<QNetworkProxy>({QNetworkProxy(getProxyType(protocols.at(i)), iterator.value().hostName, iterator.value().port)});
+							m_proxies[protocols.at(i)] = {QNetworkProxy(getProxyType(protocols.at(i)), iterator.value().hostName, iterator.value().port)};
 						}
+
 					}
 					else
 					{
@@ -70,6 +77,7 @@
 			}
 
 			break;
+#if !defined(Q_OS_RISCOS)
 		case ProxyDefinition::AutomaticProxy:
 			if (m_automaticProxy)
 			{
@@ -81,6 +89,7 @@
 			}
 
 			break;
+#endif
 		default:
 			break;
 	}
@@ -141,6 +150,7 @@
 			}
 
 			break;
+#if !defined(Q_OS_RISCOS)
 		case ProxyDefinition::AutomaticProxy:
 			if (m_automaticProxy && m_automaticProxy->isValid())
 			{
@@ -150,6 +160,7 @@
 			return QNetworkProxyFactory::systemProxyForQuery(query);
 		default:
 			break;
+#endif
 	}
 
 	return m_proxies[-1];
