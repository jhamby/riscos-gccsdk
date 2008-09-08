--- Source_Files/Misc/Logging.cpp.orig	2008-09-05 12:45:01.000134800 +0100
+++ Source_Files/Misc/Logging.cpp	2008-09-05 12:48:11.725134800 +0100
@@ -203,7 +203,12 @@
 static void
 InitializeLogging() {
     assert(sOutputFile == NULL);
+#ifdef __riscos__
+    /* Put log in application directory */
+    sOutputFile = fopen("<AlephOne$Dir>.Aleph One Log/txt", "a"); 
+#else
     sOutputFile = fopen("Aleph One Log.txt", "a");
+#endif
     sCurrentLogger = new TopLevelLogger;
     if(sOutputFile != NULL)
     {
