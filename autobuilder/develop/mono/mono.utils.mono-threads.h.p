--- mono/utils/mono-threads.h.orig	2013-01-08 18:41:09.000000000 +0000
+++ mono/utils/mono-threads.h	2013-01-19 12:27:21.000000000 +0000
@@ -78,19 +78,19 @@
 #endif
 
 enum {
-	STATE_STARTING			= 0x01,
-	STATE_RUNNING			= 0x02,
-	STATE_SHUTTING_DOWN		= 0x03,
-	STATE_DEAD				= 0x04,
-	RUN_STATE_MASK			= 0x0F,
+	mono_thread_STATE_STARTING			= 0x01,
+	mono_thread_STATE_RUNNING			= 0x02,
+	mono_thread_STATE_SHUTTING_DOWN		= 0x03,
+	mono_thread_STATE_DEAD				= 0x04,
+	mono_thread_RUN_STATE_MASK			= 0x0F,
 
-	STATE_SUSPENDED			= 0x10,
-	STATE_SELF_SUSPENDED	= 0x20,
-	SUSPEND_STATE_MASK		= 0xF0,
+	mono_thread_STATE_SUSPENDED			= 0x10,
+	mono_thread_STATE_SELF_SUSPENDED	= 0x20,
+	mono_thread_SUSPEND_STATE_MASK		= 0xF0,
 };
 
-#define mono_thread_info_run_state(info) ((info)->thread_state & RUN_STATE_MASK)
-#define mono_thread_info_suspend_state(info) ((info)->thread_state & SUSPEND_STATE_MASK)
+#define mono_thread_info_run_state(info) ((info)->thread_state & mono_thread_RUN_STATE_MASK)
+#define mono_thread_info_suspend_state(info) ((info)->thread_state & mono_thread_SUSPEND_STATE_MASK)
 
 typedef struct {
 	MonoLinkedListSetNode node;
