--- src/network/ssl/qsslsocket.cpp.orig	2015-06-29 21:04:46.000000000 +0100
+++ src/network/ssl/qsslsocket.cpp	2015-11-13 17:37:07.625457300 +0000
@@ -2582,6 +2582,7 @@
 */
 QList<QByteArray> QSslSocketPrivate::unixRootCertDirectories()
 {
+#ifndef __riscos__
     return QList<QByteArray>() <<  "/etc/ssl/certs/" // (K)ubuntu, OpenSUSE, Mandriva, MeeGo ...
                                << "/usr/lib/ssl/certs/" // Gentoo, Mandrake
                                << "/usr/share/ssl/" // Centos, Redhat, SuSE
@@ -2590,6 +2591,9 @@
                                << "/usr/local/ssl/certs/" // Solaris
                                << "/etc/openssl/certs/" // BlackBerry
                                << "/opt/openssl/certs/"; // HP-UX
+#else
+    return QList<QByteArray>() << "/<CaCertificates$Dir>/!CaCertificates/"; // RISC OS
+#endif
 }
 
 /*!
