Index: gcc/gcc.h
===================================================================
RCS file: gcc/gcc.h,v
retrieving revision 1.19
diff -u -r1.19 gcc.h
--- gcc/gcc.h	24 Nov 2004 17:47:09 -0000	1.19
+++ gcc/gcc.h	17 May 2006 21:58:42 -0000
@@ -51,6 +51,8 @@
   || !strcmp (STR, "iquote") || !strcmp (STR, "isystem") \
   || !strcmp (STR, "isysroot") \
   || !strcmp (STR, "-param") || !strcmp (STR, "specs") \
+  || !strcmp (STR, "icrossdirafter") \
+  || !strcmp (STR, "icrossdirsystem") \
   || !strcmp (STR, "MF") || !strcmp (STR, "MT") || !strcmp (STR, "MQ"))
 
 
