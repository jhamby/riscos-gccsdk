--- mono/utils/mono-threads.c.orig	2013-07-30 02:50:26.000000000 +0100
+++ mono/utils/mono-threads.c	2013-08-16 11:47:09.064280136 +0100
@@ -326,7 +326,7 @@
 	mono_mutex_lock (&info->suspend_lock);
 
 	/*thread is on the process of detaching*/
-	if (mono_thread_info_run_state (info) > STATE_RUNNING) {
+	if (mono_thread_info_run_state (info) > mono_thread_STATE_RUNNING) {
 		mono_hazard_pointer_clear (hp, 1);
 		return NULL;
 	}
@@ -350,7 +350,7 @@
 		mono_threads_core_interrupt (info);
 
 	++info->suspend_count;
-	info->thread_state |= STATE_SUSPENDED;
+	info->thread_state |= mono_thread_STATE_SUSPENDED;
 	mono_mutex_unlock (&info->suspend_lock);
 	mono_hazard_pointer_clear (hp, 1);
 
@@ -372,7 +372,7 @@
 	g_assert (info->suspend_count == 0);
 	++info->suspend_count;
 
-	info->thread_state |= STATE_SELF_SUSPENDED;
+	info->thread_state |= mono_thread_STATE_SELF_SUSPENDED;
 
 	ret = mono_threads_get_runtime_callbacks ()->thread_state_init_from_sigctx (&info->suspend_state, NULL);
 	g_assert (ret);
@@ -391,7 +391,7 @@
 mono_thread_info_resume_internal (MonoThreadInfo *info)
 {
 	gboolean result;
-	if (mono_thread_info_suspend_state (info) == STATE_SELF_SUSPENDED) {
+	if (mono_thread_info_suspend_state (info) == mono_thread_STATE_SELF_SUSPENDED) {
 		MONO_SEM_POST (&info->resume_semaphore);
 		while (MONO_SEM_WAIT (&info->finish_resume_semaphore) != 0) {
 			/* g_assert (errno == EINTR); */
@@ -400,7 +400,7 @@
 	} else {
 		result = mono_threads_core_resume (info);
 	}
-	info->thread_state &= ~SUSPEND_STATE_MASK;
+	info->thread_state &= ~mono_thread_SUSPEND_STATE_MASK;
 	return result;
 }
 
@@ -574,7 +574,7 @@
 	if (!info)
 		return;
 
-	if (mono_thread_info_run_state (info) > STATE_RUNNING) {
+	if (mono_thread_info_run_state (info) > mono_thread_STATE_RUNNING) {
 		mono_hazard_pointer_clear (hp, 1);
 		return;
 	}
