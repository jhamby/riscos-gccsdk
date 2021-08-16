--- src/network/ssl/qsslsocket.cpp.orig	2017-09-06 05:13:54.000000000 -0700
+++ src/network/ssl/qsslsocket.cpp	2021-08-14 22:26:51.467817071 -0700
@@ -2584,6 +2584,7 @@
 */
 QList<QByteArray> QSslSocketPrivate::unixRootCertDirectories()
 {
+#if !defined(Q_OS_RISCOS)
     return QList<QByteArray>() <<  "/etc/ssl/certs/" // (K)ubuntu, OpenSUSE, Mandriva, MeeGo ...
                                << "/usr/lib/ssl/certs/" // Gentoo, Mandrake
                                << "/usr/share/ssl/" // Centos, Redhat, SuSE
@@ -2593,6 +2594,9 @@
                                << "/etc/openssl/certs/" // BlackBerry
                                << "/opt/openssl/certs/" // HP-UX
                                << "/etc/ssl/"; // OpenBSD
+#else
+    return QList<QByteArray>() << "/<CaCertificates$Dir>/"; // RISC OS
+#endif
 }
 
 /*!
