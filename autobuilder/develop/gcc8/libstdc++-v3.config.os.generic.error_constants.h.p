--- libstdc++-v3/config/os/generic/error_constants.h.orig	2016-01-04 14:30:50.000000000 +0000
+++ libstdc++-v3/config/os/generic/error_constants.h	2016-11-10 11:21:16.268816477 +0000
@@ -96,7 +96,10 @@
       no_message_available = 			ENODATA, 
 #endif
 
-      no_message = 				ENOMSG, 
+#ifdef _GLIBCXX_HAVE_ENOMSG
+      no_message = 				ENOMSG,
+#endif
+
       no_protocol_option = 			ENOPROTOOPT,
       no_space_on_device = 			ENOSPC,
 
