--- mono/utils/mono-mmap.c.orig	2011-08-14 16:48:05.000000000 +0100
+++ mono/utils/mono-mmap.c	2011-08-14 16:48:58.000000000 +0100
@@ -384,7 +384,7 @@
 		madvise (addr, length, MADV_DONTNEED);
 		madvise (addr, length, MADV_FREE);
 #else
-		posix_madvise (addr, length, POSIX_MADV_DONTNEED);
+/*		posix_madvise (addr, length, POSIX_MADV_DONTNEED);*/
 #endif
 #endif
 	}
