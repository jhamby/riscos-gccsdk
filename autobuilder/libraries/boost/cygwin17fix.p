--- tools/jam/src/expand.c.old	2010-01-20 12:52:06.325557100 +0000
+++ tools/jam/src/expand.c	2010-01-20 12:51:36.139504100 +0000
@@ -711,7 +711,7 @@
     assert(l != 0);
     assert(list_next(l) == 0);
     # ifdef OS_CYGWIN
-    assert( !strcmp( l->string, "c:\\foo\\bar" ) );
+    assert( !stricmp( l->string, "c:\\foo\\bar" ) );
     # else 
     assert( !strcmp( l->string, cygpath ) );
     # endif   
