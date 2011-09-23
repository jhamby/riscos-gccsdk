--- p11-kit/modules.c.orig	2011-09-23 19:32:26.000000000 +0100
+++ p11-kit/modules.c	2011-09-23 19:32:39.000000000 +0100
@@ -253,7 +253,7 @@
 	assert (mod);
 	assert (path);
 
-	mod->dl_module = dlopen (path, RTLD_LOCAL | RTLD_NOW);
+	mod->dl_module = dlopen (path, RTLD_GLOBAL | RTLD_NOW);
 	if (mod->dl_module == NULL) {
 		_p11_message ("couldn't load module: %s: %s", path, dlerror ());
 		return CKR_GENERAL_ERROR;
