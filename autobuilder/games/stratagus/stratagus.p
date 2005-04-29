--- src/stratagus/stratagus.c.old	2004-06-26 00:22:34.000000000 +0100
+++ src/stratagus/stratagus.c	2005-04-20 14:44:08.937500000 +0100
@@ -341,7 +341,7 @@
 	return res;
 }
 
-#if !defined(BSD)
+#if !defined(BSD) || defined(__riscos__)
 /**
 **  Case insensitive version of strstr
 **
