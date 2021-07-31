--- src/processarc.c.orig	2011-05-07 16:06:10.074768721 +0100
+++ src/processarc.c	2011-05-07 16:07:35.128642852 +0100
@@ -55,6 +55,8 @@
 #include "main.h"
 #include "archives.h"
 
+static char *mkdtemp(char *template) { return NULL; }
+
 static const char *
 summarize_filename(const char *filename)
 {
