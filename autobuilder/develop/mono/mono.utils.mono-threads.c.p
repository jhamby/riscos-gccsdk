--- mono/utils/mono-threads.c.orig	2013-01-08 18:41:09.000000000 +0000
+++ mono/utils/mono-threads.c	2013-01-19 12:27:13.000000000 +0000
@@ -317,7 +317,7 @@
 	EnterCriticalSection (&info->suspend_lock);
 
 	/*thread is on the process of detaching*/
-	if (mono_thread_info_run_state (info) > STATE_RUNNING) {
+	if (mono_thread_info_run_state (info) > mono_thread_STATE_RUNNING) {
 		mono_hazard_pointer_clear (hp, 1);
 		return NULL;
 	}
@@ -341,7 +341,7 @@
 		mono_threads_core_interrupt (info);
 
 	++info->suspend_count;
-	info->thread_state |= STATE_SUSPENDED;
+	info->thread_state |= mono_thread_STATE_SUSPENDED;
 	LeaveCriticalSection (&info->suspend_lock);
 	mono_hazard_pointer_clear (hp, 1);
 
@@ -363,7 +363,7 @@
 	g_assert (info->suspend_count == 0);
 	++info->suspend_count;
 
-	info->thread_state |= STATE_SELF_SUSPENDED;
+	info->thread_state |= mono_thread_STATE_SELF_SUSPENDED;
 
 	ret = mono_threads_get_runtime_callbacks ()->thread_state_init_from_sigctx (&info->suspend_state, NULL);
 	g_assert (ret);
@@ -382,7 +382,7 @@
 mono_thread_info_resume_internal (MonoThreadInfo *info)
 {
 	gboolean result;
-	if (mono_thread_info_suspend_state (info) == STATE_SELF_SUSPENDED) {
+	if (mono_thread_info_suspend_state (info) == mono_thread_STATE_SELF_SUSPENDED) {
 		MONO_SEM_POST (&info->resume_semaphore);
 		while (MONO_SEM_WAIT (&info->finish_resume_semaphore) != 0) {
 			/* g_assert (errno == EINTR); */
@@ -391,7 +391,7 @@
 	} else {
 		result = mono_threads_core_resume (info);
 	}
-	info->thread_state &= ~SUSPEND_STATE_MASK;
+	info->thread_state &= ~mono_thread_SUSPEND_STATE_MASK;
 	return result;
 }
 
@@ -565,7 +565,7 @@
 	if (!info)
 		return;
 
-	if (mono_thread_info_run_state (info) > STATE_RUNNING) {
+	if (mono_thread_info_run_state (info) > mono_thread_STATE_RUNNING) {
 		mono_hazard_pointer_clear (hp, 1);
 		return;
 	}
