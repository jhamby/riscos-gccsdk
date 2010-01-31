--- gcc/gcc.h.orig	2007-01-18 00:25:39.000000000 +0100
+++ gcc/gcc.h	2007-01-17 03:46:14.000000000 +0100
@@ -51,6 +51,7 @@
   || !strcmp (STR, "iquote") || !strcmp (STR, "isystem") \
   || !strcmp (STR, "isysroot") \
   || !strcmp (STR, "-param") || !strcmp (STR, "specs") \
+  || !strcmp (STR, "icrossdirafter") \
   || !strcmp (STR, "MF") || !strcmp (STR, "MT") || !strcmp (STR, "MQ"))
 
 
