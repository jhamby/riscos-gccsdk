--- src/network/ssl/qsslsocket.cpp.orig	2015-09-12 16:26:43.449716617 +0100
+++ src/network/ssl/qsslsocket.cpp	2015-09-12 16:26:56.885716319 +0100
@@ -2481,6 +2481,7 @@
 */
 QList<QByteArray> QSslSocketPrivate::unixRootCertDirectories()
 {
+#ifndef __riscos__
     return QList<QByteArray>() <<  "/etc/ssl/certs/" // (K)ubuntu, OpenSUSE, Mandriva, MeeGo ...
                                << "/usr/lib/ssl/certs/" // Gentoo, Mandrake
                                << "/usr/share/ssl/" // Centos, Redhat, SuSE
@@ -2490,6 +2491,9 @@
                                << "/var/certmgr/web/user_trusted/" // BlackBerry Playbook
                                << "/etc/openssl/certs/" // BlackBerry
                                << "/opt/openssl/certs/"; // HP-UX
+#else
+    return QList<QByteArray>() << "/<Choices$Dir>/Qt5/local/share/ssl/certs/"; // RISC OS
+#endif
 }
 
 /*!
