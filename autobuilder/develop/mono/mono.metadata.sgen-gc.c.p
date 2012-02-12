--- mono/metadata/sgen-gc.c.orig	2011-09-14 04:32:35.000000000 +0100
+++ mono/metadata/sgen-gc.c	2011-12-08 20:45:30.000000000 +0000
@@ -2215,6 +2215,15 @@
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
@@ -2228,7 +2237,7 @@
 		mono_sgen_free_os_memory (mem, aligned - mem);
 	if (aligned + size < mem + size + alignment)
 		mono_sgen_free_os_memory (aligned + size, (mem + size + alignment) - (aligned + size));
-
+#endif
 	return aligned;
 }
 
@@ -3524,6 +3533,15 @@
 mono_sgen_alloc_os_memory (size_t size, int activate)
 {
 	void *ptr;
+#ifdef __riscos__
+	/* FIXME: Should we use a dynamic area? */
+	size += pagesize - 1;
+	size &= ~(pagesize - 1);
+	if ((ptr = malloc (size)) != NULL)
+	  memset (ptr, 0, size);
+	else
+	  printf("Warning: mono_sgen_alloc_os_memory returned NULL ptr\n");
+#else
 	unsigned long prot_flags = activate? MONO_MMAP_READ|MONO_MMAP_WRITE: MONO_MMAP_NONE;
 
 	prot_flags |= MONO_MMAP_PRIVATE | MONO_MMAP_ANON;
@@ -3531,6 +3549,7 @@
 	size &= ~(pagesize - 1);
 	ptr = mono_valloc (0, size, prot_flags);
 	/* FIXME: CAS */
+#endif
 	total_alloc += size;
 	return ptr;
 }
@@ -3541,11 +3560,15 @@
 void
 mono_sgen_free_os_memory (void *addr, size_t size)
 {
+#ifdef __riscos__
+	free (addr);
+#else
 	mono_vfree (addr, size);
 
 	size += pagesize - 1;
 	size &= ~(pagesize - 1);
 	/* FIXME: CAS */
+#endif
 	total_alloc -= size;
 }
 
@@ -5129,6 +5152,7 @@
 void
 mono_sgen_wait_for_suspend_ack (int count)
 {
+#ifndef __riscos__
 	int i, result;
 
 	for (i = 0; i < count; ++i) {
@@ -5138,6 +5162,7 @@
 			}
 		}
 	}
+#endif
 }
 
 static int
@@ -5146,6 +5171,7 @@
 	SgenThreadInfo *info;
 	int i, num_threads_died = 0;
 	int sleep_duration = -1;
+#ifndef __riscos__
 
 	for (;;) {
 		int restart_count = 0, restarted_count = 0;
@@ -5228,6 +5254,7 @@
 		mono_sgen_wait_for_suspend_ack (restart_count);
 #endif
 	}
+#endif
 
 	return num_threads_died;
 }
@@ -5335,7 +5362,9 @@
 	TV_GETTIME (stop_world_time);
 	count = mono_sgen_thread_handshake (suspend_signal_num);
 	count -= restart_threads_until_none_in_managed_allocator ();
+#ifndef __riscos__
 	g_assert (count >= 0);
+#endif
 	DEBUG (3, fprintf (gc_debug_file, "world stopped %d thread(s)\n", count));
 	mono_profiler_gc_event (MONO_GC_EVENT_POST_STOP_WORLD, generation);
 
