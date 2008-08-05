--- src/options.c.orig	2008-07-10 13:00:32.356283500 +0100
+++ src/options.c	2008-07-10 13:01:21.342283500 +0100
@@ -84,11 +84,15 @@
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
+        fclose(file);
+    }
 }
 
 void FinalScore()
