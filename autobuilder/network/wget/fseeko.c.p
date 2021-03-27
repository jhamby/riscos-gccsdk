--- lib/fseeko.c.orig	2021-03-27 09:47:40.589072145 +0000
+++ lib/fseeko.c	2021-03-27 09:49:17.467156715 +0000
@@ -107,6 +107,9 @@
      If this assumption is incorrect, please report the bug to
      bug-gnulib.  */
   if (0)
+#elif defined __riscos__            /* RISC OS */
+     if (fp->i_ptr == fp->i_base
+	&& fp->o_ptr == fp->o_base)
 #else
   #error "Please port gnulib fseeko.c to your platform! Look at the code in fseeko.c, then report this to bug-gnulib."
 #endif
