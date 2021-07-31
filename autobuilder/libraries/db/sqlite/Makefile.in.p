--- Makefile.in.orig	2013-04-13 20:47:16.661085832 +0100
+++ Makefile.in	2013-04-13 20:48:42.928716018 +0100
@@ -270,7 +270,7 @@
 config.h:
 	echo '#include <stdio.h>' >temp.c
 	echo 'int main(){printf(' >>temp.c
-	echo '"#define SQLITE_PTR_SZ %d",sizeof(char*));' >>temp.c
+	echo '"#define SQLITE_PTR_SZ %d",4);' >>temp.c
 	echo 'exit(0);}' >>temp.c
 	$(BCC) -o temp temp.c
 	./temp >config.h
