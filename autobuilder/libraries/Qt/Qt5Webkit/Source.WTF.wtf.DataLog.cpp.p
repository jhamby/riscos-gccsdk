--- Source/WTF/wtf/DataLog.cpp.orig	2014-05-15 18:12:23.000000000 +0100
+++ Source/WTF/wtf/DataLog.cpp	2014-07-09 20:27:03.074322620 +0100
@@ -40,13 +40,13 @@
 #endif
 #endif
 
-#define DATA_LOG_TO_FILE 0
+#define DATA_LOG_TO_FILE 1
 
 // Uncomment to force logging to the given file regardless of what the environment variable says. Note that
 // we will append ".<pid>.txt" where <pid> is the PID.
 
 // This path won't work on Windows, make sure to change to something like C:\\Users\\<more path>\\log.txt.
-#define DATA_LOG_FILENAME "/tmp/WTFLog"
+//#define DATA_LOG_FILENAME "/tmp/WTFLog"
 
 namespace WTF {
 
