--- src/network/socket/qnativesocketengine_unix.cpp.orig	2013-08-25 19:03:34.000000000 +0100
+++ src/network/socket/qnativesocketengine_unix.cpp	2013-11-13 19:49:06.986370746 +0000
@@ -602,7 +602,9 @@
             setError(QAbstractSocket::SocketResourceError, NotSocketErrorString);
             break;
         case EPROTONOSUPPORT:
+#ifndef __riscos__
         case EPROTO:
+#endif
         case EAFNOSUPPORT:
         case EINVAL:
             setError(QAbstractSocket::UnsupportedSocketOperationError, ProtocolUnsupportedErrorString);
