--- src/system/load_save.c.orig	2010-05-23 21:01:52.000000000 +0100
+++ src/system/load_save.c	2010-05-23 22:21:50.000000000 +0100
@@ -78,11 +78,17 @@
 			exit(1);
 		}
 
+		#ifdef __riscos__
+			snprintf(dir, sizeof(dir), "/<Choices$Write>/Edgar/");
+		#else
+
 		userHome = pass->pw_dir;
 
+		#endif
+
 		#ifdef __AMIGA__
 			snprintf(dir, sizeof(dir), "parallelrealities");
-		#else
+		#elif !(defined __riscos__)
 			snprintf(dir, sizeof(dir), "%s/.parallelrealities", userHome);
 		#endif
 
@@ -95,7 +101,7 @@
 
 		#ifdef __AMIGA__
 			snprintf(dir, sizeof(dir), "parallelrealities/edgar");
-		#else
+		#elif !(defined __riscos__)
 			snprintf(dir, sizeof(dir), "%s/.parallelrealities/edgar", userHome);
 		#endif
 
@@ -108,10 +114,12 @@
 
 		#ifdef __AMIGA__
 			snprintf(gameSavePath, sizeof(gameSavePath), "parallelrealities/edgar/");
-		#else
+		#elif !(defined __riscos__)
 			snprintf(gameSavePath, sizeof(gameSavePath), "%s/.parallelrealities/edgar/", userHome);
+		#else
+			snprintf(gameSavePath, sizeof(gameSavePath), "%s", dir);
 		#endif
 
 		snprintf(tempFile, sizeof(tempFile), "%stmpsave", gameSavePath);

