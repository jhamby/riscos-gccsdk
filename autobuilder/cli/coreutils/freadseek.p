--- lib/freadseek.c.orig	2009-07-25 09:12:47.000000000 -0700
+++ lib/freadseek.c	2009-07-25 09:14:34.000000000 -0700
@@ -55,6 +55,9 @@
   fp->_Next += increment;
 #elif defined __MINT__              /* Atari FreeMiNT */
   fp->__bufp += increment;
+#elif defined __riscos__
+  fp->i_ptr += increment;
+  fp->i_cnt -= increment;
 #elif defined SLOW_BUT_NO_HACKS     /* users can define this */
 #else
  #error "Please port gnulib freadseek.c to your platform! Look at the definition of getc, getc_unlocked on your system, then report this to bug-gnulib."
