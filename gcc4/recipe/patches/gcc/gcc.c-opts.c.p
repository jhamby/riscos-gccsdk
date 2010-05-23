--- gcc/c-opts.c.orig	2007-12-21 20:44:09.000000000 +0100
+++ gcc/c-opts.c	2007-12-21 20:42:12.000000000 +0100
@@ -214,6 +215,13 @@
   parse_in = cpp_create_reader (c_dialect_cxx () ? CLK_GNUCXX: CLK_GNUC89,
 				ident_hash, &line_table);
 
+#ifdef TARGET_ERROR_THROWBACK
+  {
+    cpp_callbacks *cpp_cb = cpp_get_callbacks (parse_in);
+    cpp_cb->throwback = TARGET_ERROR_THROWBACK;
+  }
+#endif
+    
   cpp_opts = cpp_get_options (parse_in);
   cpp_opts->dollars_in_ident = DOLLARS_IN_IDENTIFIERS;
   cpp_opts->objc = c_dialect_objc ();

