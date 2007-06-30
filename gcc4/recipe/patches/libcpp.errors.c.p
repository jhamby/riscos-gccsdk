--- libcpp/errors.c.orig	2007-06-30 17:07:06.000000000 +0200
+++ libcpp/errors.c	2007-06-30 17:06:38.000000000 +0200
@@ -37,14 +37,15 @@
 static void
 print_location (cpp_reader *pfile, source_location line, unsigned int col)
 {
-  if (line == 0)
+  const struct line_map *map;
+
+  if (line == 0
+      || (map = linemap_lookup (pfile->line_table, line)) == NULL)
     fprintf (stderr, "%s: ", progname);
   else
     {
-      const struct line_map *map;
       unsigned int lin;
 
-      map = linemap_lookup (pfile->line_table, line);
       linemap_print_containing_files (pfile->line_table, map);
 
       lin = SOURCE_LINE (map, line);
@@ -153,7 +154,10 @@
 	}
       else
 	{
-	  src_loc = pfile->cur_token[-1].src_loc;
+	  if (pfile->cur_run == &pfile->base_run)
+	    src_loc = 0;
+	  else
+	    src_loc = pfile->cur_token[-1].src_loc;
 	}
 
       if (_cpp_begin_message (pfile, level, src_loc, 0))
