--- gcc/gccspec.c.orig	2007-12-30 15:38:39.000000000 +0000
+++ gcc/gccspec.c	2007-12-30 14:46:58.000000000 +0000
@@ -70,6 +70,10 @@
 	}
     }
 
+#ifdef TARGET_RISCOSELF
+  shared_libgcc = 1;
+#endif
+
   if  (shared_libgcc)
     {
       /* Make sure to have room for the trailing NULL argument.  */
