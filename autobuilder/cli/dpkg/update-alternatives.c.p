--- utils/update-alternatives.c.orig	2010-09-11 19:43:53.000000000 +0100
+++ utils/update-alternatives.c	2010-09-11 19:44:32.000000000 +0100
@@ -1868,7 +1868,7 @@
 		/* Delimit name string in line */
 		i = 0;
 		name = line;
-		while (i < len && !isblank(line[i]))
+		while (i < len && !isspace(line[i]))
 			i++;
 		if (i >= len) {
 			printf("%s", prefix);
@@ -1876,12 +1876,12 @@
 			continue;
 		}
 		line[i++] = '\0';
-		while (i < len && isblank(line[i]))
+		while (i < len && isspace(line[i]))
 			i++;
 
 		/* Delimit status string in line */
 		status = line + i;
-		while (i < len && !isblank(line[i]))
+		while (i < len && !isspace(line[i]))
 			i++;
 		if (i >= len) {
 			printf("%s", prefix);
@@ -1889,7 +1889,7 @@
 			continue;
 		}
 		line[i++] = '\0';
-		while (i < len && isblank(line[i]))
+		while (i < len && isspace(line[i]))
 			i++;
 
 		/* Delimit choice string in the line */
