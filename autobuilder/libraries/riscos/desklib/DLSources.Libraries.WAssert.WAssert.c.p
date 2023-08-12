--- !DLSources/Libraries/WAssert/WAssert.c.orig	2023-08-11 17:25:22.551065857 -0700
+++ !DLSources/Libraries/WAssert/WAssert.c	2023-08-11 17:35:42.026139129 -0700
@@ -33,7 +33,9 @@
 {
 	os_error error;
 	error.errnum = 0;
-	strncpy(error.errmess, message, sizeof(error.errmess));
+	size_t messagelen = strnlen(message, sizeof(error.errmess) - 1);
+	memcpy(error.errmess, message, messagelen);
+	error.errmess[messagelen] = '\0';
 	Wimp_ReportError(&error, 1, event_taskname);
 	exit(1);
 }
