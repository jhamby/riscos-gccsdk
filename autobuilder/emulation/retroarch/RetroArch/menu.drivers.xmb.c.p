--- menu/drivers/xmb.c.orig	2020-02-15 12:13:54.441466123 +0000
+++ menu/drivers/xmb.c	2020-02-15 12:14:53.788262815 +0000
@@ -3317,6 +3317,7 @@
 static void xmb_context_reset_internal(xmb_handle_t *xmb,
       bool is_threaded, bool reinit_textures);
 
+#pragma GCC optimize("O0")
 static void xmb_render(void *data, 
       unsigned width, unsigned height, bool is_idle)
 {
@@ -3457,7 +3458,7 @@
 
    menu_animation_ctl(MENU_ANIMATION_CTL_CLEAR_ACTIVE, NULL);
 }
-
+#pragma GCC optimize("O3")
 static bool xmb_shader_pipeline_active(video_frame_info_t *video_info)
 {
    if (string_is_not_equal(menu_driver_ident(), "xmb"))
