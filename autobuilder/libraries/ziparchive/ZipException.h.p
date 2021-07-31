--- ZipArchive/ZipException.h.orig	2019-05-11 18:03:47.754509000 +0100
+++ ZipArchive/ZipException.h	2019-05-11 18:16:02.526819393 +0100
@@ -184,7 +184,7 @@
 		It is set to \c <code>GetLastError()</code> value on Windows and to \c errno on other platforms.		
 	*/
 	ZIP_SYSTEM_ERROR_TYPE m_iSystemError;
-	virtual ~CZipException();
+	virtual ~CZipException() throw();
 
 protected:
 
