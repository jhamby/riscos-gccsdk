--- src/dos/cdrom_image.cpp.orig	2004-10-05 21:55:04.000000000 +0100
+++ src/dos/cdrom_image.cpp	2005-01-06 15:45:52.406250000 +0000
@@ -30,7 +30,7 @@
 #include "cdrom.h"
 #include "drives.h"
 
-#if !defined(WIN32)
+#if !defined(WIN32) && !defined(__riscos__)
 #include <libgen.h>
 #endif
 
@@ -399,7 +399,7 @@
 	bool canAddTrack = false;
 	char tmp[MAX_FILENAME_LENGTH];	// dirname can change its argument
 	strncpy(tmp, cuefile, MAX_FILENAME_LENGTH);
-#if defined(WIN32)
+#if defined(WIN32) || defined(__riscos__)
 	string pathname("");
 #else
 	string pathname(dirname(tmp));
@@ -572,7 +572,7 @@
 	if (stat(filename.c_str(), &test) == 0) return true;
 	
 	// check if file with path relative to cue file exists
-#ifndef WIN32
+#if !defined(WIN32) && !defined(__riscos__)
 	string tmpstr(pathname + "/" + filename);
 	if (stat(tmpstr.c_str(), &test) == 0) {
 		filename = tmpstr;
