--- ./src/network/ssl/qsslsocket.cpp.orig	2015-10-12 21:35:28.000000000 -0700
+++ ./src/network/ssl/qsslsocket.cpp	2021-07-31 00:55:17.812941498 -0700
@@ -2591,6 +2591,7 @@
 */
 QList<QByteArray> QSslSocketPrivate::unixRootCertDirectories()
 {
+#if !defined(Q_OS_RISCOS)
     return QList<QByteArray>() <<  "/etc/ssl/certs/" // (K)ubuntu, OpenSUSE, Mandriva, MeeGo ...
                                << "/usr/lib/ssl/certs/" // Gentoo, Mandrake
                                << "/usr/share/ssl/" // Centos, Redhat, SuSE
@@ -2599,6 +2600,9 @@
                                << "/usr/local/ssl/certs/" // Solaris
                                << "/etc/openssl/certs/" // BlackBerry
                                << "/opt/openssl/certs/"; // HP-UX
+#else
+    return QList<QByteArray>() << "/<CaCertificates$Dir>/"; // RISC OS
+#endif
 }
 
 /*!
