--- gcc/ada/misc.c.orig	2008-04-28 16:00:00.000000000 +0200
+++ gcc/ada/misc.c	2008-04-28 15:59:41.000000000 +0200
@@ -158,6 +158,8 @@
 #define LANG_HOOKS_UNSIGNED_TYPE	gnat_unsigned_type
 #undef  LANG_HOOKS_SIGNED_OR_UNSIGNED_TYPE
 #define LANG_HOOKS_SIGNED_OR_UNSIGNED_TYPE gnat_signed_or_unsigned_type
+#undef  LANG_HOOKS_ATTRIBUTE_TABLE
+#define LANG_HOOKS_ATTRIBUTE_TABLE	gnat_internal_attribute_table
 
 const struct lang_hooks lang_hooks = LANG_HOOKS_INITIALIZER;
 
