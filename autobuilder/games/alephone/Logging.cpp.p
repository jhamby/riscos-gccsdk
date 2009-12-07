--- Source_Files/Misc/Logging.cpp.orig	2009-05-31 11:33:51.000000000 -0700
+++ Source_Files/Misc/Logging.cpp	2009-12-07 15:13:18.000000000 -0800
@@ -207,7 +207,7 @@
 	}
 }
 
-#if defined(__unix__) || defined(__NetBSD__) || defined(__OpenBSD__) || (defined(__APPLE__) && defined(__MACH__))
+#if defined(__unix__) || defined(__NetBSD__) || defined(__OpenBSD__) || (defined(__APPLE__) && defined(__MACH__)) || defined(__riscos__)
 #include <unistd.h>
 #include <sys/types.h>
 #ifdef HAVE_PWD_H
@@ -218,7 +218,7 @@
 static void
 InitializeLogging() {
     assert(sOutputFile == NULL);
-#if defined(__unix__) || defined(__NetBSD__) || defined(__OpenBSD__) || (defined(__APPLE__) && defined(__MACH__))
+#if defined(__unix__) || defined(__NetBSD__) || defined(__OpenBSD__) || (defined(__APPLE__) && defined(__MACH__)) || defined(__riscos__)
     const char *home = getenv("HOME");
     if (home == NULL) {
         struct passwd *pw = getpwuid (getuid ());
@@ -229,6 +229,8 @@
         string filename = home;
 #if defined(__APPLE__) && defined(__MACH__)
         filename += "/Library/Logs/Aleph One Log.txt";
+#elif __riscos__
+        filename += "<AlephOne$Dir>.Aleph One Log/txt";
 #else
         filename += "/.alephone/Aleph One Log.txt";
 #endif
