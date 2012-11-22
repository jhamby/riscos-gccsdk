--- libgnomevfs/xdgmimecache.h.orig	2012-11-22 17:37:30.334491445 +0000
+++ libgnomevfs/xdgmimecache.h	2012-11-22 17:38:36.862296999 +0000
@@ -48,7 +48,7 @@
 #define _xdg_mime_cache_glob_dump                     XDG_RESERVED_ENTRY(cache_glob_dump)
 #endif

-extern XdgMimeCache **_caches;
+extern XdgMimeCache **_xdg_caches;

 XdgMimeCache *_xdg_mime_cache_new_from_file (const char   *file_name);
 XdgMimeCache *_xdg_mime_cache_ref           (XdgMimeCache *cache);
