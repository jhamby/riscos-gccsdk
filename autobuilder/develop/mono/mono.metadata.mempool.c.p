--- mono/metadata/mempool.c.orig	2011-09-14 04:32:19.000000000 +0100
+++ mono/metadata/mempool.c	2011-11-13 13:57:06.000000000 +0000
@@ -317,9 +317,9 @@
 			pool->pos += size;
 		}
 	}
+#endif
 
 	return rval;
-#endif
 }
 
 /**
