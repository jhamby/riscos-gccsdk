--- src/network/socket/qnativesocketengine_unix.cpp.orig	2021-08-14 23:05:15.747192320 -0700
+++ src/network/socket/qnativesocketengine_unix.cpp	2021-08-14 23:12:37.854872694 -0700
@@ -206,10 +206,13 @@
         }
         break;
     case QNativeSocketEngine::ReceivePacketInformation:
+#if !defined(Q_OS_RISCOS)
         if (socketProtocol == QAbstractSocket::IPv6Protocol || socketProtocol == QAbstractSocket::AnyIPProtocol) {
             level = IPPROTO_IPV6;
             n = IPV6_RECVPKTINFO;
-        } else if (socketProtocol == QAbstractSocket::IPv4Protocol) {
+        } else if (socketProtocol == QAbstractSocket::IPv4Protocol)
+#endif
+        {
             level = IPPROTO_IP;
 #ifdef IP_PKTINFO
             n = IP_PKTINFO;
@@ -221,10 +224,13 @@
         }
         break;
     case QNativeSocketEngine::ReceiveHopLimit:
+#if !defined(Q_OS_RISCOS)
         if (socketProtocol == QAbstractSocket::IPv6Protocol || socketProtocol == QAbstractSocket::AnyIPProtocol) {
             level = IPPROTO_IPV6;
             n = IPV6_RECVHOPLIMIT;
-        } else if (socketProtocol == QAbstractSocket::IPv4Protocol) {
+        } else if (socketProtocol == QAbstractSocket::IPv4Protocol)
+#endif
+        {
 #ifdef IP_RECVTTL               // IP_RECVTTL is a non-standard extension supported on some OS
             level = IPPROTO_IP;
             n = IP_RECVTTL;
@@ -573,7 +579,7 @@
             setError(QAbstractSocket::SocketResourceError, NotSocketErrorString);
             break;
         case EPROTONOSUPPORT:
-#if !defined(Q_OS_OPENBSD)
+#if !defined(Q_OS_OPENBSD) && !defined(Q_OS_RISCOS)
         case EPROTO:
 #endif
         case EAFNOSUPPORT:
@@ -875,6 +881,7 @@
         struct cmsghdr *cmsgptr;
         for (cmsgptr = CMSG_FIRSTHDR(&msg); cmsgptr != NULL;
              cmsgptr = CMSG_NXTHDR(&msg, cmsgptr)) {
+#if !defined(Q_OS_RISCOS)
             if (cmsgptr->cmsg_level == IPPROTO_IPV6 && cmsgptr->cmsg_type == IPV6_PKTINFO
                     && cmsgptr->cmsg_len >= CMSG_LEN(sizeof(in6_pktinfo))) {
                 in6_pktinfo *info = reinterpret_cast<in6_pktinfo *>(CMSG_DATA(cmsgptr));
@@ -884,6 +891,7 @@
                 if (header->ifindex)
                     header->destinationAddress.setScopeId(QString::number(info->ipi6_ifindex));
             }
+#endif
 
 #ifdef IP_PKTINFO
             if (cmsgptr->cmsg_level == IPPROTO_IP && cmsgptr->cmsg_type == IP_PKTINFO
@@ -961,6 +969,7 @@
             memcpy(CMSG_DATA(cmsgptr), &header.hopLimit, sizeof(int));
             cmsgptr = reinterpret_cast<cmsghdr *>(reinterpret_cast<char *>(cmsgptr) + CMSG_SPACE(sizeof(int)));
         }
+#if !defined(Q_OS_RISCOS)
         if (header.ifindex != 0 || !header.senderAddress.isNull()) {
             struct in6_pktinfo *data = reinterpret_cast<in6_pktinfo *>(CMSG_DATA(cmsgptr));
             memset(data, 0, sizeof(*data));
@@ -974,6 +983,7 @@
             memcpy(&data->ipi6_addr, &tmp, sizeof(tmp));
             cmsgptr = reinterpret_cast<cmsghdr *>(reinterpret_cast<char *>(cmsgptr) + CMSG_SPACE(sizeof(*data)));
         }
+#endif
     } else {
         if (header.hopLimit != -1) {
             msg.msg_controllen += CMSG_SPACE(sizeof(int));
