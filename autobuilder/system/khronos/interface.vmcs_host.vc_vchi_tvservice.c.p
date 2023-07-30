--- interface/vmcs_host/vc_vchi_tvservice.c.orig	2022-11-23 17:32:16.449825383 +0000
+++ interface/vmcs_host/vc_vchi_tvservice.c	2022-11-23 17:32:16.585827099 +0000
@@ -184,6 +184,22 @@
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
@@ -192,6 +208,7 @@
 static void tvservice_notify_callback( void *callback_param,
                                       VCHI_CALLBACK_REASON_T reason,
                                       void *msg_handle );
+#endif
 
 static int32_t tvservice_wait_for_reply(void *response, uint32_t max_length, uint32_t *actual_length);
 
@@ -268,7 +285,11 @@
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
@@ -280,7 +301,11 @@
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
@@ -547,7 +572,10 @@
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
@@ -569,7 +597,10 @@
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
