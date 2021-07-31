--- physfs_platforms.h.orig	2009-07-25 17:24:33.000000000 -0700
+++ physfs_platforms.h	2009-07-25 17:25:06.000000000 -0700
@@ -38,7 +38,7 @@
 #  endif
 #elif defined(macintosh)
 #  error Classic Mac OS support was dropped from PhysicsFS 2.0. Move to OS X.
-#elif defined(unix) || defined(__unix__)
+#elif defined(unix) || defined(__unix__) || defined(__riscos__)
 #  define PHYSFS_PLATFORM_UNIX
 #  define PHYSFS_PLATFORM_POSIX
 #else
