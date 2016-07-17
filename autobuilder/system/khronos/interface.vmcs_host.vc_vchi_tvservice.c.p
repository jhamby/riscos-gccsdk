--- interface/vmcs_host/vc_vchi_tvservice.c.orig	2016-07-07 15:00:54.000000000 +0100
+++ interface/vmcs_host/vc_vchi_tvservice.c	2016-07-15 19:09:00.116217847 +0100
@@ -166,6 +166,22 @@
    vcos_mutex_unlock(&tvservice_client.lock);
 }
 
+#ifdef __riscos__
+extern void asm_tvservice_client_callback( void *callback_param,
+                                          VCHI_CALLBACK_REASON_T reason,
+                                          void *msg_handle );
+extern void asm_tvservice_notify_callback( void *callback_param,
+                                          VCHI_CALLBACK_REASON_T reason,
+                                          void *msg_handle );
+//Forward declarations
+void tvservice_client_callback( void *callback_param,
+                               VCHI_CALLBACK_REASON_T reason,
+                               void *msg_handle );
+
+void tvservice_notify_callback( void *callback_param,
+                               VCHI_CALLBACK_REASON_T reason,
+                               void *msg_handle );
+#else
 //Forward declarations
 static void tvservice_client_callback( void *callback_param,
                                       VCHI_CALLBACK_REASON_T reason,
@@ -174,6 +190,7 @@
 static void tvservice_notify_callback( void *callback_param,
                                       VCHI_CALLBACK_REASON_T reason,
                                       void *msg_handle );
+#endif
 
 static int32_t tvservice_wait_for_reply(void *response, uint32_t max_length);
 
@@ -249,7 +266,11 @@
                                                   connections[i],             // passed in fn ptrs
                                                   0,                          // tx fifo size (unused)
                                                   0,                          // tx fifo size (unused)
+#ifdef __riscos__
+                                                  &asm_tvservice_client_callback, // service callback
+#else
                                                   &tvservice_client_callback, // service callback
+#endif
                                                   &tvservice_message_available_event,  // callback parameter
                                                   VC_TRUE,                    // want_unaligned_bulk_rx
                                                   VC_TRUE,                    // want_unaligned_bulk_tx
@@ -261,7 +282,11 @@
                                                    connections[i],            // passed in fn ptrs
                                                    0,                         // tx fifo size (unused)
                                                    0,                         // tx fifo size (unused)
+#ifdef __riscos__
+                                                   &asm_tvservice_notify_callback,// service callback
+#else
                                                    &tvservice_notify_callback,// service callback
+#endif
                                                    &tvservice_notify_available_event,  // callback parameter
                                                    VC_FALSE,                  // want_unaligned_bulk_rx
                                                    VC_FALSE,                  // want_unaligned_bulk_tx
@@ -488,7 +513,10 @@
  * Description: Callback when a message is available for TV service
  *
  ***********************************************************/
-static void tvservice_client_callback( void *callback_param,
+#ifndef __riscos__
+static
+#endif
+void tvservice_client_callback( void *callback_param,
                                        const VCHI_CALLBACK_REASON_T reason,
                                        void *msg_handle ) {
    VCOS_EVENT_T *event = (VCOS_EVENT_T *)callback_param;
@@ -510,7 +538,10 @@
  * Description: Callback when a message is available for TV notify service
  *
  ***********************************************************/
-static void tvservice_notify_callback( void *callback_param,
+#ifndef __riscos__
+static
+#endif
+void tvservice_notify_callback( void *callback_param,
                                        const VCHI_CALLBACK_REASON_T reason,
                                        void *msg_handle ) {
    VCOS_EVENT_T *event = (VCOS_EVENT_T *)callback_param;
