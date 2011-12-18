--- p11-kit/compat.h.orig	2011-12-18 00:21:09.000000000 +0000
+++ p11-kit/compat.h	2011-12-18 00:21:30.000000000 +0000
@@ -124,7 +124,7 @@
 typedef void * dl_module_t;

 #define _p11_module_open(f) \
-	(dlopen ((f), RTLD_LOCAL | RTLD_NOW))
+	(dlopen ((f), RTLD_GLOBAL | RTLD_NOW))
 #define _p11_module_close(d) \
 	(dlclose(d))
 #define _p11_module_error() \
