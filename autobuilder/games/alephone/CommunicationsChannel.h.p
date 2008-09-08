--- Source_Files/TCPMess/CommunicationsChannel.h.orig	2005-10-01 22:54:33.000000000 +0100
+++ Source_Files/TCPMess/CommunicationsChannel.h	2008-09-04 11:26:54.853000000 +0100
@@ -95,7 +95,12 @@
 	// Calls back message handlers (if appropriate)
 	void		dispatchIncomingMessages();
 
+#ifdef __riscos__
+      /* This change is really a GCC4 difference */
+	bool		isMessageAvailable();
+#else
 	bool		CommunicationsChannel::isMessageAvailable();
+#endif
 
 	// Call does not return unless (1) times out (NULL); (2) disconnected (NULL); or
 	// (3) some message received (pointer to inflated message object).
