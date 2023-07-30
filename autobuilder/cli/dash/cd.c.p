--- src/cd.c.orig	2023-07-30 13:52:00.664800710 -0700
+++ src/cd.c	2023-07-30 13:57:21.239671909 -0700
@@ -133,7 +133,7 @@
 		p = stalloc(len);
 
 		if (stat64(p, &statb) >= 0 && S_ISDIR(statb.st_mode)) {
-			if (c && c != ':')
+			if (c && c != ';') /* change from colon for RISC OS 'FS::' */
 				flags |= CD_PRINT;
 docd:
 			if (!docd(p, flags))
