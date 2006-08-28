--- heap.c.org	2004-10-19 16:51:21.000000000 +0200
+++ heap.c	2006-07-23 16:43:56.000000000 +0200
@@ -94,7 +94,8 @@
     }
   }
 #else
-#if HAVE_MMAP && USE_MMAP
+//#if HAVE_MMAP && USE_MMAP
+#if 0
 #if MAP_ANON
   /* if we have and can use mmap, then do so */
   ret = mmap(0L, incr, PROT_READ | PROT_WRITE | PROT_EXEC,
