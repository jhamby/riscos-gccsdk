--- src/processarc.c.orig	2010-06-10 23:33:59.851231321 +0100
+++ src/processarc.c	2010-06-10 23:36:01.131234394 +0100
@@ -52,6 +52,8 @@
 #include "main.h"
 #include "archives.h"
 
+char *mkdtemp(char *template) { return NULL; };
+
 struct rename_list {
   struct rename_list *next;
   char *src;
