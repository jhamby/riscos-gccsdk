--- mono/io-layer/io.c.orig	2012-02-03 20:48:56.000000000 +0000
+++ mono/io-layer/io.c	2012-02-03 21:15:03.000000000 +0000
@@ -43,6 +43,12 @@
 #include <mono/io-layer/io-portability.h>
 #include <mono/utils/strenc.h>
 
+#ifdef __riscos__
+#include <kernel.h>
+#include <unixlib/local.h>
+#define RISCOS_CIL_IMAGE_FILETYPE 0xA7D
+#endif
+
 #undef DEBUG
 
 static void file_close (gpointer handle, gpointer data);
@@ -1686,6 +1692,18 @@
 	g_message("%s: returning handle %p", __func__, handle);
 #endif
 	
+#ifdef __riscos__
+	if ((flags & (O_CREAT | O_WRONLY))
+	 && (strstr (filename, ".exe") || strstr (filename, "/exe"))) {
+		char riscos_filename [_POSIX_PATH_MAX];
+		if (__riscosify_std (filename, 0, riscos_filename, sizeof (riscos_filename), NULL)) {
+			_kernel_osfile_block file;
+
+			file.load = RISCOS_CIL_IMAGE_FILETYPE;
+			_kernel_osfile (18, riscos_filename, &file);
+		}
+	}
+#endif
 	return(handle);
 }
 
