--- pager.c.orig	2014-12-18 18:42:18.000000000 +0000
+++ pager.c	2015-06-05 12:54:10.461971332 +0100
@@ -3,8 +3,16 @@
 #include "sigchain.h"
 
 #ifndef DEFAULT_PAGER
+#ifdef __riscos__
+/* RISC OS has automatic paging in the command mode, but most
+ * people will be running it in a task window which will give
+ * scroll bars to look back at earlier entries
+ */
+#define DEFAULT_PAGER NULL
+#else
 #define DEFAULT_PAGER "less"
 #endif
+#endif
 
 struct pager_config {
 	const char *cmd;
