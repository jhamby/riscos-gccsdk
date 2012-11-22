--- libgnomevfs/xdgmimecache.c.orig	2012-11-22 17:37:59.646405781 +0000
+++ libgnomevfs/xdgmimecache.c	2012-11-22 17:41:45.749744551 +0000
@@ -320,9 +320,9 @@
   const char *ptr;
   int i, min, max, mid, cmp;

-  for (i = 0; _caches[i]; i++)
+  for (i = 0; _xdg_caches[i]; i++)
     {
-      XdgMimeCache *cache = _caches[i];
+      XdgMimeCache *cache = _xdg_caches[i];
       xdg_uint32_t list_offset = GET_UINT32 (cache->buffer, 4);
       xdg_uint32_t n_entries = GET_UINT32 (cache->buffer, list_offset);
       xdg_uint32_t offset;
@@ -366,9 +366,9 @@
   const char *ptr;
   int i, min, max, mid, cmp;

-  for (i = 0; _caches[i]; i++)
+  for (i = 0; _xdg_caches[i]; i++)
     {
-      XdgMimeCache *cache = _caches[i];
+      XdgMimeCache *cache = _xdg_caches[i];
       xdg_uint32_t list_offset = GET_UINT32 (cache->buffer, 12);
       xdg_uint32_t n_entries = GET_UINT32 (cache->buffer, list_offset);
       xdg_uint32_t offset;
@@ -419,9 +419,9 @@
   int i, j, n;

   n = 0;
-  for (i = 0; _caches[i]; i++)
+  for (i = 0; _xdg_caches[i]; i++)
     {
-      XdgMimeCache *cache = _caches[i];
+      XdgMimeCache *cache = _xdg_caches[i];

       xdg_uint32_t list_offset = GET_UINT32 (cache->buffer, 20);
       xdg_uint32_t n_entries = GET_UINT32 (cache->buffer, list_offset);
@@ -539,9 +539,9 @@
 {
   int i, n;

-  for (i = 0; _caches[i]; i++)
+  for (i = 0; _xdg_caches[i]; i++)
     {
-      XdgMimeCache *cache = _caches[i];
+      XdgMimeCache *cache = _xdg_caches[i];

       xdg_uint32_t list_offset = GET_UINT32 (cache->buffer, 16);
       xdg_uint32_t n_entries = GET_UINT32 (cache->buffer, list_offset);
@@ -647,9 +647,9 @@
   int i;

   max_extent = 0;
-  for (i = 0; _caches[i]; i++)
+  for (i = 0; _xdg_caches[i]; i++)
     {
-      XdgMimeCache *cache = _caches[i];
+      XdgMimeCache *cache = _xdg_caches[i];

       offset = GET_UINT32 (cache->buffer, 24);
       max_extent = MAX (max_extent, GET_UINT32 (cache->buffer, offset + 4));
@@ -670,9 +670,9 @@

   priority = 0;
   mime_type = NULL;
-  for (i = 0; _caches[i]; i++)
+  for (i = 0; _xdg_caches[i]; i++)
     {
-      XdgMimeCache *cache = _caches[i];
+      XdgMimeCache *cache = _xdg_caches[i];

       int prio;
       const char *match;
@@ -847,9 +847,9 @@
   if (strcmp (ubase, "application/octet-stream") == 0)
     return 1;

-  for (i = 0; _caches[i]; i++)
+  for (i = 0; _xdg_caches[i]; i++)
     {
-      XdgMimeCache *cache = _caches[i];
+      XdgMimeCache *cache = _xdg_caches[i];

       xdg_uint32_t list_offset = GET_UINT32 (cache->buffer, 8);
       xdg_uint32_t n_entries = GET_UINT32 (cache->buffer, list_offset);
@@ -910,9 +910,9 @@
   mime = xdg_mime_unalias_mime_type (mime);

   p = 0;
-  for (i = 0; _caches[i]; i++)
+  for (i = 0; _xdg_caches[i]; i++)
     {
-      XdgMimeCache *cache = _caches[i];
+      XdgMimeCache *cache = _xdg_caches[i];

       xdg_uint32_t list_offset = GET_UINT32 (cache->buffer, 8);
       xdg_uint32_t n_entries = GET_UINT32 (cache->buffer, list_offset);
@@ -962,9 +962,9 @@
   const char *ptr;
   int i, min, max, mid, cmp;

-  for (i = 0; _caches[i]; i++)
+  for (i = 0; _xdg_caches[i]; i++)
     {
-      XdgMimeCache *cache = _caches[i];
+      XdgMimeCache *cache = _xdg_caches[i];
       xdg_uint32_t list_offset = GET_UINT32 (cache->buffer, header);
       xdg_uint32_t n_entries = GET_UINT32 (cache->buffer, list_offset);
       xdg_uint32_t offset;
@@ -1038,9 +1038,9 @@
 _xdg_mime_cache_glob_dump (void)
 {
   int i, j;
-  for (i = 0; _caches[i]; i++)
+  for (i = 0; _xdg_caches[i]; i++)
   {
-    XdgMimeCache *cache = _caches[i];
+    XdgMimeCache *cache = _xdg_caches[i];
     xdg_uint32_t list_offset;
     xdg_uint32_t n_entries;
     xdg_uint32_t offset;
