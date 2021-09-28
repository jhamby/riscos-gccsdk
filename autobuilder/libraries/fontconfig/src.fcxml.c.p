--- src/fcxml.c.orig	2021-09-28 15:59:23.000000000 +0100
+++ src/fcxml.c	2021-09-28 16:05:05.320216185 +0100
@@ -3464,6 +3464,15 @@
 	goto bail1;
 
     do {
+#ifdef __ARM_EABI__
+	/* Unfortunately, the read below causes a stack page fault from SVC mode
+	 * when a SWI (probably OS_GBPB) is used to read a file to the stack.
+	 * Work around it by forcing the stack pages to be mapped in from USR mode
+	 * first. Hopefully, this will be a temporary fix until ARMEABISupport
+	 * uses OS_AbortTrap instead of my inferior custom routines.
+	 */
+	memset (buf, 0, BUFSIZ);
+#endif
 	len = read (fd, buf, BUFSIZ);
 	if (len < 0)
 	{
