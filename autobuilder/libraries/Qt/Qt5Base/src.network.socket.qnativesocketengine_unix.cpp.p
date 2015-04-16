--- src/network/socket/qnativesocketengine_unix.cpp.orig	2015-02-17 04:56:38.000000000 +0000
+++ src/network/socket/qnativesocketengine_unix.cpp	2015-04-12 13:43:51.612657031 +0100
@@ -622,7 +622,9 @@
             setError(QAbstractSocket::SocketResourceError, NotSocketErrorString);
             break;
         case EPROTONOSUPPORT:
+#ifndef __riscos__
         case EPROTO:
+#endif
         case EAFNOSUPPORT:
         case EINVAL:
             setError(QAbstractSocket::UnsupportedSocketOperationError, ProtocolUnsupportedErrorString);
