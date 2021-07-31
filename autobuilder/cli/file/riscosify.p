--- src/file.c.orig	2008-03-09 17:58:06.000000000 -0700
+++ src/file.c	2008-03-09 17:59:56.000000000 -0700
@@ -57,7 +57,13 @@
 #ifdef HAVE_LOCALE_H
 #include <locale.h>
 #endif
-#ifdef HAVE_WCHAR_H
+
+#ifdef __riscos__
+#include <unixlib/local.h>
+int __feature_image_fs_is_file = 1;
+#endif
+
+#ifdef HAVE_WCHAR_H
 #include <wchar.h>
 #endif

@@ -316,6 +322,10 @@
                break;
        }

+#ifdef __riscos__
+       __set_riscosify_control(__RISCOSIFY_NO_PROCESS);
+#endif
+
        if (optind == argc) {
                if (!didsomefiles)
                        usage();
