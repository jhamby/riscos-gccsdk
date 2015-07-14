--- interface/khronos/common/khrn_client.h.orig	2015-06-18 16:07:56.860394693 +0100
+++ interface/khronos/common/khrn_client.h	2015-06-10 21:29:46.000000000 +0100
@@ -115,7 +115,9 @@
    */
 
    bool high_priority;
-
+#ifdef __riscos__
+   uint8_t pad[3];
+#endif
    uint8_t merge_buffer[MERGE_BUFFER_SIZE];
 
    uint32_t merge_pos;
