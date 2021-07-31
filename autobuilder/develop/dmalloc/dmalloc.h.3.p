--- dmalloc.h.3.org	2004-10-19 16:51:21.000000000 +0200
+++ dmalloc.h.3	2006-11-12 23:36:19.000000000 +0100
@@ -43,6 +43,7 @@
 #define DMALLOC_FUNC_MEMALIGN	14	/* memalign function called */
 #define DMALLOC_FUNC_VALLOC	15	/* valloc function called */
 #define DMALLOC_FUNC_STRDUP	16	/* strdup function called */
+#define DMALLOC_FUNC_STRNDUP	19	/* strndup function called */
 #define DMALLOC_FUNC_FREE	17	/* free function called */
 #define DMALLOC_FUNC_CFREE	18	/* cfree function called */
 
