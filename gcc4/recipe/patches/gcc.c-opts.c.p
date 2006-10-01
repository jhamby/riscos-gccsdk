--- gcc/c-opts.c.orig	2006-09-24 23:20:22.000000000 +0200
+++ gcc/c-opts.c	2006-09-24 23:20:31.000000000 +0200
@@ -158,6 +158,7 @@
 
     case OPT_F:
     case OPT_I:
+    case OPT_icrossdirafter: /* NAB */
     case OPT_idirafter:
     case OPT_isysroot:
     case OPT_isystem:
@@ -811,6 +812,26 @@
       add_path (xstrdup (arg), AFTER, 0, true);
       break;
 
+      /* NAB++ */
+    case OPT_icrossdirafter:
+      {
+	char *path;
+	const char *prefix;
+	const char *suffix = arg;
+	size_t prefix_len, suffix_len;
+
+	suffix_len = strlen (suffix);
+	prefix     = cpp_CROSS_INCLUDE_DIR;
+	prefix_len = cpp_CROSS_INCLUDE_DIR_len;
+
+	path = xmalloc (prefix_len + suffix_len + 1);
+	strcpy (path, prefix);
+	strcat (path, suffix);
+	add_path (path, AFTER, 0, true);
+      }
+      break;
+      /* NAB-- */
+
     case OPT_imacros:
     case OPT_include:
       defer_opt (code, arg);
