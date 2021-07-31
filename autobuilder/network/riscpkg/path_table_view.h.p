--- !RiscPkg/path_table_view.h.orig	2010-09-11 11:36:36.000000000 +0100
+++ !RiscPkg/path_table_view.h	2010-09-11 11:37:04.000000000 +0100
@@ -113,7 +113,7 @@
 	/** Get the only selected path, if there is one.
 	 * @return the only selected path, or 0 if there is more or less than one
 	 */
-	value_type* path_table_view::only_selection();
+	value_type* only_selection();
 };
 
 /** A structure for recording the content and selection state of one row. */
