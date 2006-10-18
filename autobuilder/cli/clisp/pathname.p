--- src/pathname_base.d	2006-01-13 02:33:47.000000000 +0000
+++ src/pathname.d	2006-08-27 18:32:15.000000000 +0100
@@ -7003,7 +7948,7 @@
        #if defined(UNIX_CYGWIN32)
         /* Neither d_reclen nor d_namlen present in DIR structure. */
         direntry_len = asciz_length(dp->d_name);
-       #elif !defined(HAVE_STRUCT_DIRENT_D_NAMLEN) || defined(__USE_GNU)
+       #elif !defined(HAVE_STRUCT_DIRENT_D_NAMLEN) || (defined(__USE_GNU) && defined(d_namlen))
         { /* On UNIX_LINUX direntry_len := dp->d_reclen was sufficient, but in
            general direntry_len := min(dp->d_reclen,asciz_length(dp->d_name))
            is necessary. The GNU libc is buggy: it does
