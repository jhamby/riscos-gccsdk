--- mono/utils/hazard-pointer.c.orig	2013-01-18 17:54:32.000000000 +0000
+++ mono/utils/hazard-pointer.c	2013-01-18 17:55:25.000000000 +0000
@@ -85,6 +85,11 @@
 		hazard_table = g_malloc0 (sizeof (MonoThreadHazardPointers) * HAZARD_TABLE_MAX_SIZE);
 		hazard_table_size = HAZARD_TABLE_MAX_SIZE;
 #else
+#ifdef __riscos__
+		/* FIXME: Should we use a dynamic area for this kind of thing?  */
+		hazard_table = g_malloc0 (sizeof (MonoThreadHazardPointers) * HAZARD_TABLE_MAX_SIZE);
+		hazard_table_size = HAZARD_TABLE_MAX_SIZE;
+#else
 		gpointer page_addr;
 		int pagesize = mono_pagesize ();
 		int num_pages = (hazard_table_size * sizeof (MonoThreadHazardPointers) + pagesize - 1) / pagesize;
@@ -104,6 +109,7 @@
 		hazard_table_size = num_pages * pagesize / sizeof (MonoThreadHazardPointers);
 
 #endif
+#endif
 		g_assert (id < hazard_table_size);
 		for (i = 0; i < HAZARD_POINTER_COUNT; ++i)
 			hazard_table [id].hazard_pointers [i] = NULL;
