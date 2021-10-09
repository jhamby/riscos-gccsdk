--- lib/freading.c.orig	2021-10-09 11:19:27.779809071 +0100
+++ lib/freading.c	2021-10-09 11:20:43.086762074 +0100
@@ -68,6 +68,8 @@
   if (fp->state == 0 /* CLOSED */ || fp->state == 4 /* WR */)
     return 0;
   return (fp->state == 3 /* RD */ && (fp->bufl == 0 || fp->rp < fp->wp));
+# elif defined __riscos__            /* RISC OS */
+    return (fp->__mode.__bits.__read) != 0;
 # else
 #  error "Please port gnulib freading.c to your platform!"
 # endif
