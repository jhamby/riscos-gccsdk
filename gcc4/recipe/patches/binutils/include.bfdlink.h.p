--- include/bfdlink.h.orig	2006-11-14 18:04:52.000000000 +0000
+++ include/bfdlink.h	2006-11-14 18:09:58.000000000 +0000
@@ -422,6 +422,9 @@
 
   /* Start and end of RELRO region.  */
   bfd_vma relro_start, relro_end;
+
+  /* 1 for -fpic, 2 for fPIC (RISC OS) */
+  unsigned int flag_pic;
 };
 
 /* This structures holds a set of callback functions.  These are
