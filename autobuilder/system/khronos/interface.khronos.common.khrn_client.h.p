--- interface/khronos/common/khrn_client.h.orig	2016-06-17 15:32:01.098576532 +0100
+++ interface/khronos/common/khrn_client.h	2016-07-07 10:56:29.306813830 +0100
@@ -115,8 +115,11 @@
    */
 
    bool high_priority;
-
+#ifdef __riscos__
+   uint8_t *merge_buffer;
+#else
    uint8_t merge_buffer[MERGE_BUFFER_SIZE];
+#endif
 
    uint32_t merge_pos;
    uint32_t merge_end;
@@ -124,6 +127,12 @@
 	/* Try to reduce impact of repeated consecutive glGetError() calls */
 	int32_t glgeterror_hack;
 	bool async_error_notification;
+
+#ifdef __riscos__
+   /* Memory used for transfering data to the GPU. This needs to be uncached to prevent
+      data corruption. */
+   void *uncached_buffer;
+#endif
 };
 
 extern void client_thread_state_init(CLIENT_THREAD_STATE_T *state);
