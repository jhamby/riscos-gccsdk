--- eglib/src/gmisc-unix.c.orig	2012-11-10 12:21:46.000000000 +0000
+++ eglib/src/gmisc-unix.c	2012-11-10 14:36:31.000000000 +0000
@@ -69,7 +69,11 @@
 {
 	g_return_val_if_fail (filename != NULL, FALSE);
 
+#ifdef __riscos__
+	return (*filename == '/' || strchr (filename, '$'));
+#else
 	return (*filename == '/');
+#endif
 }
 
 static pthread_mutex_t pw_lock = PTHREAD_MUTEX_INITIALIZER;
