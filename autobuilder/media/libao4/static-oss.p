--- src/Makefile.in.orig	2008-03-09 14:01:48.000000000 -0700
+++ src/Makefile.in	2008-03-09 14:02:09.000000000 -0700
@@ -55,7 +55,7 @@
 	ao_au.c ao_raw.c ao_aixs.c ao_private.h ao_wmm.c
 @HAVE_WMM_TRUE@am__objects_1 = ao_wmm.lo
 am_libao_la_OBJECTS = audio_out.lo config.lo ao_null.lo ao_wav.lo \
-	ao_au.lo ao_raw.lo ao_aixs.lo $(am__objects_1)
+	ao_au.lo ao_raw.lo ao_aixs.lo $(am__objects_1) plugins/oss/ao_oss.lo
 libao_la_OBJECTS = $(am_libao_la_OBJECTS)
 libao_la_LINK = $(LIBTOOL) --tag=CC $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) \
 	--mode=link $(CCLD) $(AM_CFLAGS) $(CFLAGS) $(libao_la_LDFLAGS) \
--- src/plugins/oss/ao_oss.c.rej	2008-03-09 14:13:11.000000000 -0700
+++ src/plugins/oss/ao_oss.c	2008-03-09 14:13:34.000000000 -0700
@@ -331,3 +331,14 @@
 	free(internal->dev);
 	free(internal);
 }
+
+ao_functions ao_oss = {
+       ao_plugin_test,
+       ao_plugin_driver_info,
+       ao_plugin_device_init,
+       ao_plugin_set_option,
+       ao_plugin_open,
+       ao_plugin_play,
+       ao_plugin_close,
+       ao_plugin_device_clear
+};
--- src/audio_out.c.orig	2008-03-09 14:29:10.000000000 -0700
+++ src/audio_out.c	2008-03-09 14:29:52.000000000 -0700
@@ -78,6 +78,8 @@
 #ifdef HAVE_WMM
 extern ao_functions ao_wmm;
 #endif
+extern ao_functions ao_oss;
+
 static ao_functions *static_drivers[] = {
 	&ao_null, /* Must have at least one static driver! */
 	&ao_wav,
@@ -89,13 +91,14 @@
 #ifdef HAVE_WMM
 	&ao_wmm,
 #endif
+	&ao_oss,
 
 	NULL /* End of list */
 };
 
 static driver_list *driver_head = NULL;
 static ao_config config = {
-	NULL /* default_driver */
+	"oss" /* default_driver */
 };
 
 static ao_info **info_table = NULL;
@@ -584,6 +587,7 @@
 
 	if (!driver_head) return;
 
+#if 0
 	/* unload and free all the drivers */
 	while (driver) {
 		if (driver->handle) {
@@ -599,6 +603,7 @@
         _clear_config();
 	/* NULL out driver_head or ao_initialize() won't work */
 	driver_head = NULL;
+#endif
 }
 
 
