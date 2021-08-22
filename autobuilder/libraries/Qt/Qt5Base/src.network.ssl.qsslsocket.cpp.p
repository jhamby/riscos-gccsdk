--- src/network/ssl/qsslsocket.cpp.orig	2021-05-17 23:43:52.000000000 -0700
+++ src/network/ssl/qsslsocket.cpp	2021-08-16 13:31:39.764657530 -0700
@@ -2792,6 +2792,7 @@
 */
 QList<QByteArray> QSslSocketPrivate::unixRootCertDirectories()
 {
+#if !defined(Q_OS_RISCOS)
     return QList<QByteArray>() <<  "/etc/ssl/certs/" // (K)ubuntu, OpenSUSE, Mandriva ...
                                << "/usr/lib/ssl/certs/" // Gentoo, Mandrake
                                << "/usr/share/ssl/" // Centos, Redhat, SuSE
@@ -2801,6 +2802,9 @@
                                << "/etc/openssl/certs/" // BlackBerry
                                << "/opt/openssl/certs/" // HP-UX
                                << "/etc/ssl/"; // OpenBSD
+#else
+    return QList<QByteArray>() << "/<CaCertificates$Dir>/"; // RISC OS
+#endif
 }
 
 /*!
