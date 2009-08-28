--- modules/access/mmap.c.orig	2009-08-28 12:25:50.000000000 -0700
+++ modules/access/mmap.c	2009-08-28 12:26:05.000000000 -0700
@@ -141,7 +141,7 @@
             goto error;
     }
 
-    p_sys->page_size = sysconf (_SC_PAGE_SIZE);
+    p_sys->page_size = 4096; //sysconf (_SC_PAGE_SIZE);
     p_sys->mtu = MMAP_SIZE;
     if (p_sys->mtu < p_sys->page_size)
         p_sys->mtu = p_sys->page_size;
