--- ZipArchive/ZipPlatform_lnx.cpp.orig	2009-04-24 16:47:02.000000000 +0100
+++ ZipArchive/ZipPlatform_lnx.cpp	2009-04-09 11:41:51.856156700 +0100
@@ -12,16 +12,18 @@
 // Web Site: http://www.artpol-software.com
 ////////////////////////////////////////////////////////////////////////////////
 
-#include "stdafx.h"
+#include "_platform.h"
 
 #ifdef _ZIP_SYSTEM_LINUX
 
-#if defined __APPLE__ || defined __CYGWIN__
+// Note: RISC OS doesn't currently support 64 bit versions of functions
+#if defined __APPLE__ || defined __CYGWIN__ || defined __riscos__
 	#define FILE_FUNCTIONS_64B_BY_DEFAULT
 #else
 	#undef FILE_FUNCTIONS_64B_BY_DEFAULT	
 #endif	
 
+#include "stdafx.h"
 #include "ZipPlatform.h"
 #include "ZipFileHeader.h"
 #include "ZipException.h"
