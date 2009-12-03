--- src/system/load_save.c.orig	2009-12-03 13:46:15.000000000 -0800
+++ src/system/load_save.c	2009-12-03 13:51:06.000000000 -0800
@@ -75,9 +75,13 @@
 			exit(1);
 		}
 
+#ifdef __riscos__
+		snprintf(dir, sizeof(dir), "/<Choices$Write>/Edgar/");
+#else
 		userHome = pass->pw_dir;
 
 		snprintf(dir, sizeof(dir), "%s/.parallelrealities", userHome);
+#endif
 
 		if ((mkdir(dir, S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH) != 0) && (errno != EEXIST))
 		{
@@ -86,6 +90,7 @@
 			exit(1);
 		}
 
+#ifndef __riscos__
 		snprintf(dir, sizeof(dir), "%s/.parallelrealities/edgar", userHome);
 
 		if ((mkdir(dir, S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH) != 0) && (errno != EEXIST))
@@ -96,6 +101,9 @@
 		}
 
 		snprintf(gameSavePath, sizeof(gameSavePath), "%s/.parallelrealities/edgar/", userHome);
+#else
+		snprintf(gameSavePath, sizeof(gameSavePath), "%s", dir);
+#endif
 
 		snprintf(tempFile, sizeof(tempFile), "%stmpsave", gameSavePath);
 
