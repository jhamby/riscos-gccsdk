--- lib/freadseek.c.orig	2015-04-29 21:11:40.280379202 +0100
+++ lib/freadseek.c	2015-04-29 21:20:05.008389626 +0100
@@ -62,6 +62,9 @@
   fp->__bufp += increment;
 #elif defined EPLAN9                /* Plan9 */
   fp->rp += increment;
+#elif defined __riscos__
+  fp->i_ptr += increment;
+  fp->i_cnt -= increment;
 #elif defined SLOW_BUT_NO_HACKS     /* users can define this */
 #else
  #error "Please port gnulib freadseek.c to your platform! Look at the definition of getc, getc_unlocked on your system, then report this to bug-gnulib."
