--- kaffe/kaffe/main.c.old	2003-01-28 18:54:44.000000000 +0000
+++ kaffe/kaffe/main.c	2003-01-28 18:54:02.000000000 +0000
@@ -83,7 +83,7 @@
 	JNI_GetDefaultJavaVMInitArgs(&vmargs);
 
 	/* set up libtool/libltdl dlopen emulation */
-	LTDL_SET_PRELOADED_SYMBOLS();
+/*	LTDL_SET_PRELOADED_SYMBOLS();*/
 
 #if defined(DEBUG)
 	cp = getenv("KAFFE_VMDEBUG");
