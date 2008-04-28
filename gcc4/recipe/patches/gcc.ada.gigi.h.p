--- gcc/ada/gigi.h.orig	2008-04-28 16:00:05.000000000 +0200
+++ gcc/ada/gigi.h	2008-04-28 15:59:41.000000000 +0200
@@ -309,7 +309,6 @@
    type whose bit width is Pmode.  Assume "long" is such a type here.  */
 #undef SIZE_TYPE
 #define SIZE_TYPE "long int"
-
 
 /* Data structures used to represent attributes.  */
 
@@ -332,6 +331,9 @@
   Node_Id error_point;
 };
 
+/* Table of machine-independent internal attributes.  */
+extern const struct attribute_spec gnat_internal_attribute_table[];
+
 /* Define the entries in the standard data array.  */
 enum standard_datatypes
 {
