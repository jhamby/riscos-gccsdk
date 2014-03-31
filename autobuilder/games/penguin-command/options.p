--- src/options.c.orig	2014-03-31 12:07:45.462030188 +0100
+++ src/options.c	2014-03-31 12:09:29.978030571 +0100
@@ -9,7 +9,12 @@
 {
     char filename[200];
     FILE *file;
-    
+
+#ifdef __riscos__
+    if ((char *)getenv("Choices$Write") != NULL)
+        snprintf(filename, sizeof(filename), "/%s/.penguin-command", getenv("Choices$Write"));
+    else
+#endif   
     if ((char *)getenv("HOME") != NULL)
 		snprintf( filename, sizeof(filename), "%s/.penguin-command", getenv("HOME"));
     else
@@ -84,11 +89,15 @@
 	||(fscanf(file, "%d\n",&HighDetail)==EOF)
 	||(fscanf(file, "%d\n",&MusicVol)==EOF)
 	||(fscanf(file, "%d\n",&SoundVol)==EOF))
+    {
+        fclose(file);
 	WriteOptions();
 //    for (i=0;i<10;i++)
 //	printf("%s %d\n",Player[i],Hiscore[i]);
-    
-    fclose(file);
+    } else
+    {   
+       fclose(file);
+    }
 }
 
 void FinalScore()
