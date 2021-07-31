--- src/network/socket/qnativesocketengine_unix.cpp.orig	2015-06-29 21:04:47.000000000 +0100
+++ src/network/socket/qnativesocketengine_unix.cpp	2015-07-15 18:24:19.938735896 +0100
@@ -589,7 +589,9 @@
             setError(QAbstractSocket::SocketResourceError, NotSocketErrorString);
             break;
         case EPROTONOSUPPORT:
+#ifndef __riscos__
         case EPROTO:
+#endif
         case EAFNOSUPPORT:
         case EINVAL:
             setError(QAbstractSocket::UnsupportedSocketOperationError, ProtocolUnsupportedErrorString);
