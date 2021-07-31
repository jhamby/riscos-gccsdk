From a02883528979bbefc6d6d4be79d10b853e7b7041 Mon Sep 17 00:00:00 2001
From: Carsten Teibes <dev@f4ke.de>
Date: Sun, 6 Nov 2016 19:50:07 +0100
Subject: Fix libxmp-lite building (wrong format loaders) Currently trying to
 use the lite version fails with undefined references to the old format loader
 names.

This bug was introduced in d018decfb98d: "Add prefix to format loaders"

diff --git a/lite/src/format.c b/lite/src/format.c
index 277a3da..e19f070 100644
--- src/format.c
+++ src/format.c
@@ -27,20 +27,20 @@
 #endif
 #include "format.h"
 
-extern const struct format_loader xm_loader;
-extern const struct format_loader mod_loader;
-extern const struct format_loader it_loader;
-extern const struct format_loader s3m_loader;
+extern const struct format_loader libxmp_loader_xm;
+extern const struct format_loader libxmp_loader_mod;
+extern const struct format_loader libxmp_loader_it;
+extern const struct format_loader libxmp_loader_s3m;
 
 extern const struct pw_format *const pw_format[];
 
 const struct format_loader *const format_loader[5] = {
-	&xm_loader,
-	&mod_loader,
+	&libxmp_loader_xm,
+	&libxmp_loader_mod,
 #ifndef LIBXMP_CORE_DISABLE_IT
-	&it_loader,
+	&libxmp_loader_it,
 #endif
-	&s3m_loader,
+	&libxmp_loader_s3m,
 	NULL
 };
 
diff --git a/lite/src/loaders/mod_load.c b/lite/src/loaders/mod_load.c
index b7a8f8d..bf72f36 100644
--- src/loaders/mod_load.c
+++ src/loaders/mod_load.c
@@ -36,7 +36,7 @@
 static int mod_test (HIO_HANDLE *, char *, const int);
 static int mod_load (struct module_data *, HIO_HANDLE *, const int);
 
-const struct format_loader mod_loader = {
+const struct format_loader libxmp_loader_mod = {
     "Protracker",
     mod_test,
     mod_load
