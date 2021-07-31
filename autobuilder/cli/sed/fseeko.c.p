--- gnulib-tests/fseeko.c.orig	2021-01-13 19:21:07.659278408 +0000
+++ gnulib-tests/fseeko.c	2021-01-13 19:26:26.384996899 +0000
@@ -101,6 +101,9 @@
 #elif defined EPLAN9                /* Plan9 */
   if (fp->rp == fp->buf
       && fp->wp == fp->buf)
+#elif defined __riscos__            /* RISC OS */
+  if (fp->i_ptr == fp->i_base
+      && fp->o_ptr == fp->o_base)
 #elif FUNC_FFLUSH_STDIN < 0 && 200809 <= _POSIX_VERSION
   /* Cross-compiling to some other system advertising conformance to
      POSIX.1-2008 or later.  Assume fseeko and fflush work as advertised.
