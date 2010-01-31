--- gcc/ada/utils.c.orig	2008-04-28 15:59:55.000000000 +0200
+++ gcc/ada/utils.c	2008-04-28 15:59:41.000000000 +0200
@@ -79,6 +79,21 @@
 tree static_ctors;
 tree static_dtors;
 
+/* Forward declarations for handlers of attributes.  */
+static tree handle_const_attribute (tree *, tree, tree, int, bool *);
+static tree handle_nothrow_attribute (tree *, tree, tree, int, bool *);
+
+/* Table of machine-independent internal attributes for Ada.  We support
+   this minimal set ot attributes to accomodate the Alpha back-end which
+   unconditionally puts them on its builtins.  */
+const struct attribute_spec gnat_internal_attribute_table[] =
+{
+  /* { name, min_len, max_len, decl_req, type_req, fn_type_req, handler } */
+  { "const",   0, 0, true,  false, false, handle_const_attribute   },
+  { "nothrow", 0, 0, true,  false, false, handle_nothrow_attribute },
+  { NULL,      0, 0, false, false, false, NULL }
+};
+
 /* Associates a GNAT tree node to a GCC tree node. It is used in
    `save_gnu_tree', `get_gnu_tree' and `present_gnu_tree'. See documentation
    of `save_gnu_tree' for more info.  */
@@ -1826,11 +1841,48 @@
   gnat_pushdecl (decl, Empty);
   DECL_BUILT_IN_CLASS (decl) = class;
   DECL_FUNCTION_CODE (decl) = function_code;
+
+  /* Possibly apply some default attributes to this built-in function.  */
   if (attrs)
-      decl_attributes (&decl, attrs, ATTR_FLAG_BUILT_IN);
+    decl_attributes (&decl, attrs, ATTR_FLAG_BUILT_IN);
+  else
+    decl_attributes (&decl, NULL_TREE, 0);
+
   return decl;
 }
 
+/* Handle a "const" attribute; arguments as in
+   struct attribute_spec.handler.  */
+
+static tree
+handle_const_attribute (tree *node, tree ARG_UNUSED (name),
+			tree ARG_UNUSED (args), int ARG_UNUSED (flags),
+			bool *no_add_attrs)
+{
+  if (TREE_CODE (*node) == FUNCTION_DECL)
+    TREE_READONLY (*node) = 1;
+  else
+    *no_add_attrs = true;
+
+  return NULL_TREE;
+}
+
+/* Handle a "nothrow" attribute; arguments as in
+   struct attribute_spec.handler.  */
+
+static tree
+handle_nothrow_attribute (tree *node, tree ARG_UNUSED (name),
+			  tree ARG_UNUSED (args), int ARG_UNUSED (flags),
+			  bool *no_add_attrs)
+{
+  if (TREE_CODE (*node) == FUNCTION_DECL)
+    TREE_NOTHROW (*node) = 1;
+  else
+    *no_add_attrs = true;
+
+  return NULL_TREE;
+}
+
 /* Return an integer type with the number of bits of precision given by
    PRECISION.  UNSIGNEDP is nonzero if the type is unsigned; otherwise
    it is a signed type.  */
