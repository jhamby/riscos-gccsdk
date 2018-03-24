--- src/cd.c.orig	2018-03-22 20:04:13.908379000 +1300
+++ src/cd.c	2018-03-22 20:09:38.827592391 +1300
@@ -128,7 +128,7 @@
 		c = *path;
 		p = padvance(&path, dest);
 		if (stat(p, &statb) >= 0 && S_ISDIR(statb.st_mode)) {
-			if (c && c != ':')
+			if (c && c != ';') /* change from colon for RISC OS 'FS::' */
 				flags |= CD_PRINT;
 docd:
 			if (!docd(p, flags))
