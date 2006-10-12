--- ./dmalloc_tab.c.org	2004-10-19 15:51:21.000000000 +0100
+++ ./dmalloc_tab.c	2006-10-11 22:28:42.310000000 +0100
@@ -793,7 +793,7 @@
       /* can we still print the pointer information? */
       if (log_n == 0 || entry_c < log_n) {
 	(void)_dmalloc_chunk_desc_pnt(source, sizeof(source),
-				      tab_p->mt_file, tab_p->mt_line);
+				      tab_p->mt_file, tab_p->mt_line, 0);
 	log_entry(tab_p, in_use_column_b, source);
       }
       add_entry(&total, tab_p);

