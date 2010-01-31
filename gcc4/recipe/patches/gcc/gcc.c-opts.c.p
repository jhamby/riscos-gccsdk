--- gcc/c-opts.c.orig	2007-12-21 20:44:09.000000000 +0100
+++ gcc/c-opts.c	2007-12-21 20:42:12.000000000 +0100
@@ -158,6 +158,7 @@
 
     case OPT_F:
     case OPT_I:
+    case OPT_icrossdirafter:
     case OPT_idirafter:
     case OPT_isysroot:
     case OPT_isystem:
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
@@ -811,6 +819,36 @@
       add_path (xstrdup (arg), AFTER, 0, true);
       break;
 
+    case OPT_icrossdirafter:
+      {
+	char *path;
+	const char *suffix = arg;
+
+	/* We assume the crossdirafter option comes *after* iprefix option.
+	   Here we translate the standard prefix of first part of
+	   CROSS_INCLUDE_DIR with iprefix value.  */
+	if (iprefix != NULL
+	    && !strncmp (cpp_CROSS_INCLUDE_DIR, cpp_GCC_INCLUDE_DIR, cpp_GCC_INCLUDE_DIR_len))
+	  {
+	    path = concat (iprefix, cpp_CROSS_INCLUDE_DIR + cpp_GCC_INCLUDE_DIR_len, suffix, NULL);
+	  }
+	else
+	  {
+	    const char *prefix;
+	    size_t prefix_len, suffix_len;
+
+	    prefix     = cpp_CROSS_INCLUDE_DIR;
+	    prefix_len = cpp_CROSS_INCLUDE_DIR_len;
+
+	    suffix_len = strlen (suffix);
+	    path = xmalloc (prefix_len + suffix_len + 1);
+	    strcpy (path, prefix);
+	    strcat (path, suffix);
+	  }
+	add_path (path, AFTER, 0 /* cxx_aware */, true);
+      }
+      break;
+
     case OPT_imacros:
     case OPT_include:
       defer_opt (code, arg);
