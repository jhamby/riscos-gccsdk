--- mono/metadata/threads.c.orig	2011-12-19 21:10:25.000000000 +0000
+++ mono/metadata/threads.c	2012-02-13 19:12:51.000000000 +0000
@@ -395,6 +395,11 @@
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
@@ -412,7 +417,7 @@
 
 		++num_pages;
 		hazard_table_size = num_pages * pagesize / sizeof (MonoThreadHazardPointers);
-
+#endif
 #endif
 		g_assert (id < hazard_table_size);
 		hazard_table [id].hazard_pointers [0] = NULL;
