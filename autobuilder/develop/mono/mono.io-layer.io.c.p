--- mono/io-layer/io.c.orig	2013-01-08 18:41:09.000000000 +0000
+++ mono/io-layer/io.c	2013-01-21 19:47:00.000000000 +0000
@@ -46,6 +46,12 @@
 #include <mono/io-layer/io-portability.h>
 #include <mono/utils/strenc.h>
 
+#ifdef __riscos__
+#include <kernel.h>
+#include <unixlib/local.h>
+#define RISCOS_CIL_IMAGE_FILETYPE 0xA7D
+#endif
+
 #if 0
 #define DEBUG(...) g_message(__VA_ARGS__)
 #define DEBUG_ENABLED 1
@@ -1574,6 +1580,18 @@
 	
 	DEBUG("%s: returning handle %p", __func__, handle);
 	
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
 
@@ -4194,7 +4212,7 @@
 }
 
 /* Linux has struct statfs which has a different layout */
-#if defined (PLATFORM_MACOSX) || defined (__linux__) || defined(PLATFORM_BSD) || defined(__native_client__)
+#if defined (PLATFORM_MACOSX) || defined (__linux__) || defined(PLATFORM_BSD) || defined(__native_client__) || defined (__riscos__)
 gboolean
 GetVolumeInformation (const gunichar2 *path, gunichar2 *volumename, int volumesize, int *outserial, int *maxcomp, int *fsflags, gunichar2 *fsbuffer, int fsbuffersize)
 {
