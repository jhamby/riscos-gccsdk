--- modules/stream_filter/decomp.c.orig	2009-08-28 11:53:10.000000000 -0700
+++ modules/stream_filter/decomp.c	2009-08-28 11:53:55.000000000 -0700
@@ -28,7 +28,7 @@
 #include <vlc_network.h>
 #include <assert.h>
 #include <unistd.h>
-#ifndef _POSIX_SPAWN
+#if !defined(_POSIX_SPAWN) || defined(__riscos__)
 # define _POSIX_SPAWN (-1)
 #endif
 #include <fcntl.h>
