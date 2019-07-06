--- ZipArchive/ZipException.cpp.orig	2019-05-11 18:03:47.754509000 +0100
+++ ZipArchive/ZipException.cpp	2019-05-11 18:17:10.155392967 +0100
@@ -41,7 +41,7 @@
 #endif
 }
 
-CZipException::~CZipException()
+CZipException::~CZipException() throw()
 {
 
 }
