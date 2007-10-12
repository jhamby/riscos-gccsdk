--- gas/read.c.orig	2007-10-07 14:05:47.000000000 +0200
+++ gas/read.c	2007-10-09 22:49:22.000000000 +0200
@@ -1279,7 +1279,7 @@
   unsigned int align_limit = ALIGN_LIMIT;
   unsigned int align;
   char *stop = NULL;
-  char stopc;
+  char stopc = 0;
   offsetT fill = 0;
   int max;
   int fill_p;
@@ -1423,7 +1423,7 @@
   offsetT temp, size;
   symbolS *symbolP = NULL;
   char *stop = NULL;
-  char stopc;
+  char stopc = 0;
   expressionS exp;
 
   if (flag_mri)
@@ -1538,7 +1538,7 @@
   symbolS *sym;
   offsetT align;
   char *stop = NULL;
-  char stopc;
+  char stopc = 0;
 
   if (!flag_mri)
     {
@@ -1807,7 +1807,7 @@
 {
   offsetT temp;
   char *stop = NULL;
-  char stopc;
+  char stopc = 0;
 
   if (flag_mri)
     stop = mri_comment_field (&stopc);
@@ -1929,7 +1929,7 @@
   int c;
   symbolS *symbolP;
   char *stop = NULL;
-  char stopc;
+  char stopc = 0;
 
   if (flag_mri)
     stop = mri_comment_field (&stopc);
@@ -2881,7 +2881,7 @@
   expressionS val;
   char *p = 0;
   char *stop = NULL;
-  char stopc;
+  char stopc = 0;
   int bytes;
 
 #ifdef md_flush_pending_output
@@ -3057,7 +3057,7 @@
   int flen;
   char temp[MAXIMUM_NUMBER_OF_CHARS_FOR_FLOAT];
   char *stop = NULL;
-  char stopc;
+  char stopc = 0;
 
   if (flag_mri)
     stop = mri_comment_field (&stopc);
@@ -3134,7 +3134,7 @@
 s_struct (int ignore ATTRIBUTE_UNUSED)
 {
   char *stop = NULL;
-  char stopc;
+  char stopc = 0;
 
   if (flag_mri)
     stop = mri_comment_field (&stopc);
@@ -3514,7 +3514,7 @@
   int c;
   expressionS exp;
   char *stop = NULL;
-  char stopc;
+  char stopc = 0;
 
 #ifdef md_flush_pending_output
   md_flush_pending_output ();
@@ -5010,7 +5010,7 @@
 equals (char *sym_name, int reassign)
 {
   char *stop = NULL;
-  char stopc;
+  char stopc = 0;
 
   input_line_pointer++;
   if (*input_line_pointer == '=')
