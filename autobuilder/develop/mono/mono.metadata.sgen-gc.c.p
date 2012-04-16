--- mono/metadata/sgen-gc.c.orig	2011-12-19 21:10:25.000000000 +0000
+++ mono/metadata/sgen-gc.c	2012-04-15 17:30:18.000000000 +0100
@@ -2214,6 +2214,15 @@
 void*
 mono_sgen_alloc_os_memory_aligned (mword size, mword alignment, gboolean activate)
 {
+#ifdef __riscos__
+	/* FIXME: This is wasteful of memory. The original implementation below
+	   moves the allocated block to an address of the requested alignment,
+	   then returns any unused memory before and after the block back to the OS.
+	   We might be able to do this in RISC OS with a dynamic area.  */
+	char *mem = mono_sgen_alloc_os_memory (size + alignment, activate);
+	char *aligned = (char*)((mword)(mem + (alignment - 1)) & ~(alignment - 1));
+	g_assert (aligned >= mem && aligned + size <= mem + size + alignment && !((mword)aligned & (alignment - 1)));
+#else
 	/* Allocate twice the memory to be able to put the block on an aligned address */
 	char *mem = mono_sgen_alloc_os_memory (size + alignment, activate);
 	char *aligned;
@@ -2227,7 +2236,7 @@
 		mono_sgen_free_os_memory (mem, aligned - mem);
 	if (aligned + size < mem + size + alignment)
 		mono_sgen_free_os_memory (aligned + size, (mem + size + alignment) - (aligned + size));
-
+#endif
 	return aligned;
 }
 
@@ -2503,14 +2512,14 @@
 static void
 stw_bridge_process (void)
 {
-	g_assert (mono_sgen_need_bridge_processing ());
+//	g_assert (mono_sgen_need_bridge_processing ());
 	mono_sgen_bridge_processing_stw_step ();
 }
 
 static void
 bridge_process (void)
 {
-	g_assert (mono_sgen_need_bridge_processing ());
+//	g_assert (mono_sgen_need_bridge_processing ());
 	mono_sgen_bridge_processing_finish ();
 }
 
@@ -3553,6 +3562,16 @@
 mono_sgen_alloc_os_memory (size_t size, int activate)
 {
 	void *ptr;
+#ifdef __riscos__
+	size_t alloc_size = (size + (pagesize - 1)) & ~(pagesize - 1);
+
+	/* FIXME: Should we use a dynamic area? */
+
+	if ((ptr = mono_riscos_memory_alloc (alloc_size)) != NULL)
+	  memset (ptr, 0, size);
+	else
+	  printf("Warning: mono_sgen_alloc_os_memory returned NULL ptr\n");
+#else
 	unsigned long prot_flags = activate? MONO_MMAP_READ|MONO_MMAP_WRITE: MONO_MMAP_NONE;
 
 	prot_flags |= MONO_MMAP_PRIVATE | MONO_MMAP_ANON;
@@ -3560,6 +3579,7 @@
 	size &= ~(pagesize - 1);
 	ptr = mono_valloc (0, size, prot_flags);
 	/* FIXME: CAS */
+#endif
 	total_alloc += size;
 	return ptr;
 }
@@ -3570,11 +3590,15 @@
 void
 mono_sgen_free_os_memory (void *addr, size_t size)
 {
+#ifdef __riscos__
+	mono_riscos_memory_free (addr);
+#else
 	mono_vfree (addr, size);
 
 	size += pagesize - 1;
 	size &= ~(pagesize - 1);
 	/* FIXME: CAS */
+#endif
 	total_alloc -= size;
 }
 
@@ -5169,6 +5193,7 @@
 void
 mono_sgen_wait_for_suspend_ack (int count)
 {
+#ifndef __riscos__
 	int i, result;
 
 	for (i = 0; i < count; ++i) {
@@ -5178,6 +5203,7 @@
 			}
 		}
 	}
+#endif
 }
 
 static int
@@ -5186,6 +5212,7 @@
 	SgenThreadInfo *info;
 	int i, num_threads_died = 0;
 	int sleep_duration = -1;
+#ifndef __riscos__
 
 	for (;;) {
 		int restart_count = 0, restarted_count = 0;
@@ -5268,6 +5295,7 @@
 		mono_sgen_wait_for_suspend_ack (restart_count);
 #endif
 	}
+#endif
 
 	return num_threads_died;
 }
@@ -5378,7 +5406,9 @@
 	TV_GETTIME (stop_world_time);
 	count = mono_sgen_thread_handshake (suspend_signal_num);
 	count -= restart_threads_until_none_in_managed_allocator ();
+#ifndef __riscos__
 	g_assert (count >= 0);
+#endif
 	DEBUG (3, fprintf (gc_debug_file, "world stopped %d thread(s)\n", count));
 	mono_profiler_gc_event (MONO_GC_EVENT_POST_STOP_WORLD, generation);
 
