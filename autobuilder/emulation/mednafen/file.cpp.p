--- srcfile.cpp.orig	2012-03-31 09:27:50.000000000 +0100
+++ src/file.cpp	2012-03-31 09:28:32.000000000 +0100
@@ -490,7 +490,7 @@
     }
     memcpy(tmp_ptr, fceufp->data, fceufp->size);

-    munmap(fceufp->data, fceufp->size);
+    munmap((char *)fceufp->data, fceufp->size);

     fceufp->is_mmap = FALSE;
     fceufp->data = (uint8 *)tmp_ptr;
@@ -517,7 +517,7 @@
  {
   #if HAVE_MMAP
   if(fp->is_mmap)
-   munmap(fp->data, fp->size);
+   munmap((char *)fp->data, fp->size);
   else
   #endif
    free(fp->data);
