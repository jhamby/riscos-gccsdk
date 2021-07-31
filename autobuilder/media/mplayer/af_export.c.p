--- libaf/af_export.c.orig	2012-02-19 13:37:43.000000000 +0000
+++ libaf/af_export.c	2012-02-19 13:37:54.000000000 +0000
@@ -127,7 +127,7 @@
     // Initialize header
     *((int*)s->mmap_area) = af->data->nch;
     *((int*)s->mmap_area + 1) = s->sz * af->data->bps * af->data->nch;
-    msync(s->mmap_area, mapsize, MS_ASYNC);
+    msync(s->mmap_area, mapsize);
 
     // Use test_output to return FALSE if necessary
     return af_test_output(af, (af_data_t*)arg);
