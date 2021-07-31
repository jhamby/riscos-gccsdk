--- mono/metadata/mempool.c.orig	2013-01-08 18:41:08.000000000 +0000
+++ mono/metadata/mempool.c	2013-02-24 15:58:14.000000000 +0000
@@ -269,11 +269,11 @@
 
 #ifdef MALLOC_ALLOCATION
 	{
-		Chunk *c = g_malloc (size);
+		Chunk *c = g_malloc (sizeof (Chunk) + size);
 
 		c->next = pool->chunks;
 		pool->chunks = c;
-		c->size = size - sizeof(Chunk);
+		c->size = size;
 
 		pool->allocated += size;
 
