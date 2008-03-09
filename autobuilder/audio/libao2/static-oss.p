Index: src/plugins/oss/ao_oss.c
===================================================================
--- src/plugins/oss/ao_oss.c	(revision 8508)
+++ src/plugins/oss/ao_oss.c	(working copy)
@@ -330,3 +330,15 @@
 	free(internal->dev);
 	free(internal);
 }
+
+ao_functions ao_oss = {
+	ao_plugin_test,
+	ao_plugin_driver_info,
+	ao_plugin_device_init,
+	ao_plugin_set_option,
+	ao_plugin_open,
+	ao_plugin_play,
+	ao_plugin_close,
+	ao_plugin_device_clear
+};
+
Index: src/Makefile.am
===================================================================
--- src/Makefile.am	(revision 8508)
+++ src/Makefile.am	(working copy)
@@ -7,7 +7,7 @@
 
 lib_LTLIBRARIES = libao.la
 
-libao_la_SOURCES = audio_out.c config.c ao_null.c ao_wav.c ao_au.c ao_raw.c ao_aixs.c ao_private.h
+libao_la_SOURCES = audio_out.c config.c ao_null.c ao_wav.c ao_au.c ao_raw.c ao_aixs.c ao_private.h plugins/oss/ao_oss.c
 libao_la_LDFLAGS = -version-info @LIB_CURRENT@:@LIB_REVISION@:@LIB_AGE@ -ldl
 
 
Index: src/audio_out.c
===================================================================
--- src/audio_out.c	(revision 8508)
+++ src/audio_out.c	(working copy)
@@ -28,7 +28,7 @@
 #include <stdio.h>
 #include <string.h>
 #include <limits.h>
-#include <dlfcn.h>
+//#include <dlfcn.h>
 #include <sys/types.h>
 #include <sys/stat.h>
 #ifndef _MSC_VER
@@ -63,6 +63,7 @@
 extern ao_functions ao_wav;
 extern ao_functions ao_raw;
 extern ao_functions ao_au;
+extern ao_functions ao_oss;
 #ifdef HAVE_SYS_AUDIO_H
 extern ao_functions ao_aixs;
 #endif
@@ -72,6 +73,7 @@
 	&ao_wav,
 	&ao_raw,
 	&ao_au,
+	&ao_oss,
 #ifdef HAVE_SYS_AUDIO_H
 	&ao_aixs,
 #endif
@@ -105,6 +107,7 @@
 	driver_list *dt;
 	void *handle;
 
+#if 0
 	handle = dlopen(plugin_file, DLOPEN_FLAG /* See ao_private.h */);
 
 	if (handle) {
@@ -157,6 +160,7 @@
  failed:
 	free(dt->functions);
 	free(dt);
+#endif
 	return NULL;
 }
 
@@ -563,7 +567,7 @@
 	/* unload and free all the drivers */
 	while (driver) {
 		if (driver->handle) {
-		  dlclose(driver->handle);
+		  //dlclose(driver->handle);
 		  free(driver->functions); /* DON'T FREE STATIC FUNC TABLES */
 		}
 		next_driver = driver->next;
--- src/Makefile.in	2004-12-21 21:35:50.000000000 +0000
+++ src/Makefile.in.new	2004-12-21 21:35:45.000000000 +0000
@@ -51,7 +51,7 @@
 LTLIBRARIES = $(lib_LTLIBRARIES)
 libao_la_LIBADD =
 am_libao_la_OBJECTS = audio_out.lo config.lo ao_null.lo ao_wav.lo \
-	ao_au.lo ao_raw.lo ao_aixs.lo
+	ao_au.lo ao_raw.lo ao_aixs.lo plugins/oss/ao_oss.lo
 libao_la_OBJECTS = $(am_libao_la_OBJECTS)
 DEFAULT_INCLUDES = -I. -I$(srcdir)
 depcomp = $(SHELL) $(top_srcdir)/depcomp
