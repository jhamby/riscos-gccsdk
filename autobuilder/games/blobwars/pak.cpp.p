--- src/pak.cpp.orig	2021-03-06 13:22:06.538778837 +0000
+++ src/pak.cpp	2021-03-06 13:24:02.614008573 +0000
@@ -119,7 +119,7 @@
 			{
 				printf("Couldn't open %s for reading!\n", filename);
 				closedir(dirp);
-				gzclose(pak);
+				gzclose((gzFile)pak);
 				exit(1);
 			}
 			
@@ -141,7 +141,7 @@
 			{
 				printf("Couldn't open %s for reading!\n", filename);
 				closedir(dirp);
-				gzclose(pak);
+				gzclose((gzFile)pak);
 				exit(1);
 			}
 			else
