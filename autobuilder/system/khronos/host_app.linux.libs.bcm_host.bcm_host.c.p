--- host_applications/linux/libs/bcm_host/bcm_host.c.orig	2016-05-28 10:25:01.443146915 +0100
+++ host_applications/linux/libs/bcm_host/bcm_host.c	2016-05-28 10:25:01.787146925 +0100
@@ -36,6 +36,7 @@
 static VCHI_INSTANCE_T global_initialise_instance;
 static VCHI_CONNECTION_T *global_connection;
 
+#ifndef __riscos__
 int32_t graphics_get_display_size( const uint16_t display_number,
                                                     uint32_t *width,
                                                     uint32_t *height)
@@ -68,6 +69,7 @@
 
    return success;
 }
+#endif
 
 void host_app_message_handler(void)
 {
@@ -82,27 +84,30 @@
 
 void bcm_host_init(void)
 {
+#ifndef __riscos__
    VCHIQ_INSTANCE_T vchiq_instance;
+#endif
    static int initted;
    int success = -1;
-   char response[ 128 ];
-   
+//   char response[ 128 ];
+
    if (initted)
 	return;
    initted = 1;
    vcos_init();
-
+#ifndef __riscos__
    if (vchiq_initialise(&vchiq_instance) != VCHIQ_SUCCESS)
    {
       printf("* failed to open vchiq instance\n");
       exit(-1);
    }
-
+#endif
    vcos_log("vchi_initialise");
    success = vchi_initialise( &global_initialise_instance);
    vcos_assert(success == 0);
+#ifndef __riscos__
    vchiq_instance = (VCHIQ_INSTANCE_T)global_initialise_instance;
-
+#endif
    global_connection = vchi_create_connection(single_get_func_table(),
                                               vchi_mphi_message_driver_func_table());
 
@@ -115,11 +120,13 @@
    vc_vchi_cec_init (global_initialise_instance, &global_connection, 1);
    //vc_vchi_bufman_init (global_initialise_instance, &global_connection, 1);
 
+#ifndef __riscos__
    if ( success == 0 )
    {
       success = vc_gencmd( response, sizeof(response), "set_vll_dir /sd/vlls" );
       vcos_assert( success == 0 );
    }
+#endif
 }
 
 void bcm_host_deinit(void)
