--- gcc/c-opts.c.orig	2010-07-04 16:15:11.274402439 +0200
+++ gcc/c-opts.c	2010-07-04 16:14:32.714402060 +0200
@@ -79,6 +79,9 @@ static const char *deps_file;
 /* The prefix given by -iprefix, if any.  */
 static const char *iprefix;
 
+/* The multilib directory given by -imultilib, if any.  */
+static const char *imultilib;
+
 /* The system root, if any.  Overridden by -isysroot.  */
 static const char *sysroot = TARGET_SYSTEM_ROOT;
 
@@ -214,6 +217,13 @@ c_common_init_options (unsigned int argc
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
@@ -816,6 +826,10 @@ c_common_handle_option (size_t scode, co
       defer_opt (code, arg);
       break;
 
+    case OPT_imultilib:
+      imultilib = arg;
+      break;
+
     case OPT_iprefix:
       iprefix = arg;
       break;
@@ -970,7 +984,7 @@ c_common_post_options (const char **pfil
 
   sanitize_cpp_opts ();
 
-  register_include_chains (parse_in, sysroot, iprefix,
+  register_include_chains (parse_in, sysroot, iprefix, imultilib,
 			   std_inc, std_cxx_inc && c_dialect_cxx (), verbose);
 
   flag_inline_trees = 1;
