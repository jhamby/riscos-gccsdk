--- src/filesubr_no_riscos.c	2005-05-07 18:09:21.000000000 +0200
+++ src/filesubr.c	2005-05-07 18:06:03.000000000 +0200
@@ -36,6 +36,7 @@
     if (noexec)
 	return;
 
+#ifdef HAVE_READLINK
     /* If the file to be copied is a link or a device, then just create
        the new link or device appropriately. */
     if (islink (from))
@@ -45,6 +46,7 @@
 	free (source);
 	return;
     }
+#endif
 
     if (isdevice (from))
     {
@@ -581,6 +583,7 @@
 
     /* If FILE1 and FILE2 are symlinks, they are equal if they point to
        the same thing. */
+#ifdef HAVE_READLINK
 #ifdef S_ISLNK
     if (S_ISLNK (sb1.st_mode) && S_ISLNK (sb2.st_mode))
     {
@@ -593,6 +596,7 @@
 	return result;
     }
 #endif
+#endif
 
     /* If FILE1 and FILE2 are devices, they are equal if their device
        numbers match. */
