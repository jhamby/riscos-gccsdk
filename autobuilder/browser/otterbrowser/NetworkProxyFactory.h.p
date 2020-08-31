--- src/core/NetworkProxyFactory.h.orig	2020-08-30 18:02:47.378227936 +0100
+++ src/core/NetworkProxyFactory.h	2020-08-30 18:05:13.975274727 +0100
@@ -1,6 +1,6 @@
 /**************************************************************************
 * Otter Browser: Web browser controlled by the user, not vice-versa.
-* Copyright (C) 2013 - 2017 Michal Dutkiewicz aka Emdek <michal@emdek.pl>
+* Copyright (C) 2013 - 2018 Michal Dutkiewicz aka Emdek <michal@emdek.pl>
 * Copyright (C) 2014 - 2017 Jan Bajer aka bajasoft <jbajer@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
@@ -46,7 +46,9 @@
 	QNetworkProxy::ProxyType getProxyType(ProxyDefinition::ProtocolType protocol);
 
 private:
+#if !defined(Q_OS_RISCOS)
 	NetworkAutomaticProxy *m_automaticProxy;
+#endif
 	ProxyDefinition m_definition;
 	QMap<int, QList<QNetworkProxy> > m_proxies;
 };
