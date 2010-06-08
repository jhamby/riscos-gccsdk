--- lib/getgroups.c.orig	2010-06-08 22:27:07.185668997 +0100
+++ lib/getgroups.c	2010-06-08 22:27:33.758164699 +0100
@@ -25,7 +25,7 @@
 #include <stdlib.h>
 #include <stdint.h>
 
-#if !HAVE_GETGROUPS
+#if !HAVE_GETGROUPS && !(defined __riscos__)
 
 /* Provide a stub that fails with ENOSYS, since there is no group
    information available on mingw.  */
