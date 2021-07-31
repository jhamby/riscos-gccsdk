--- interface/khronos/common/khrn_client.c.orig	2016-06-17 15:31:45.570576086 +0100
+++ interface/khronos/common/khrn_client.c	2016-07-01 13:51:46.135331863 +0100
@@ -54,6 +54,10 @@
 #include "applications/vmcs/khronos/khronos_server.h"
 #endif
 
+#ifdef __riscos__
+#include "main.h"
+#endif
+
 VCOS_LOG_CAT_T khrn_client_log = VCOS_LOG_INIT("khrn_client", VCOS_LOG_WARN);
 
 /*
@@ -296,6 +300,9 @@
 
 	state->glgeterror_hack = 0;
 	state->async_error_notification = false;
+
+   state->merge_buffer = globals->uncached_memory_command;
+   state->uncached_buffer = state->merge_buffer + ((MERGE_BUFFER_SIZE + 0xfff) & ~0xfff);
 }
 
 void client_thread_state_term(CLIENT_THREAD_STATE_T *state)
