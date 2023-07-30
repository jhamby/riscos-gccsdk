--- src/video/raspberry/SDL_rpivideo.c.orig	2022-11-23 22:00:20.988089000 +0000
+++ src/video/raspberry/SDL_rpivideo.c	2022-11-23 22:01:47.761152969 +0000
@@ -39,6 +39,7 @@
 #include "../../events/SDL_mouse_c.h"
 #include "../../events/SDL_keyboard_c.h"
 #include "SDL_hints.h"
+#include "SDL_assert.h"
 
 #ifdef SDL_INPUT_LINUXEV
 #include "../../core/linux/SDL_evdev.h"
@@ -57,9 +58,14 @@
     SDL_free(device);
 }
 
+#ifdef __RISCOS__
+#include "oslib/osbyte.h"
+#endif
+
 static int 
 RPI_GetRefreshRate()
 {
+#ifndef __RISCOS__
     TV_DISPLAY_STATE_T tvstate;
     if (vc_tv_get_display_state( &tvstate ) == 0) {
         //The width/height parameters are in the same position in the union
@@ -71,6 +77,7 @@
             tvstate.display.hdmi.frame_rate * (1000.0f/1001.0f) : 
             tvstate.display.hdmi.frame_rate;
     } 
+#endif
     return 60;  /* Failed to get display state, default to 60 */
 }
 
@@ -215,6 +222,33 @@
     
     RPI_InitMouse(_this);
 
+#ifdef __RISCOS__
+    {
+        SDL_VideoData *driverdata = (SDL_VideoData *) _this->driverdata;
+	os_VDU_VAR_LIST(5) var_list = { { os_VDUVAR_XEIG_FACTOR,
+					  os_VDUVAR_YEIG_FACTOR,
+					  os_VDUVAR_XWIND_LIMIT,
+					  os_VDUVAR_YWIND_LIMIT,
+					  os_VDUVAR_END_LIST } };
+	int i;
+
+	_swix(OS_ReadVduVariables, _INR(0,1), &var_list, &var_list);
+	driverdata->eigen_x = var_list.var[0];
+	driverdata->eigen_y = var_list.var[1];
+	driverdata->screen_width = var_list.var[2] + 1;
+	driverdata->screen_height = var_list.var[3] + 1;
+	driverdata->old_mouse_buttons = 0;
+
+	for (i = 0; i < RISCOS_MAX_KEYS_PRESSED; i++)
+	    driverdata->key_pressed[i] = 0;
+
+	/* Turn off the RISC OS pointer, otherwise it causes the graphics to stutter.  */
+	_swi(OS_CLI, _IN(0), "Pointer 0");
+
+	/* Disable escape.  */
+	_swi(OSByte, _INR(0,2), osbyte_VAR_ESCAPE_STATE, 1, 0);
+    }
+#endif
     return 1;
 }
 
@@ -224,6 +258,13 @@
 #ifdef SDL_INPUT_LINUXEV    
     SDL_EVDEV_Quit();
 #endif    
+#ifdef __RISCOS__
+    /* Turn the RISC OS pointer back on.  */
+    _swi(OS_CLI, _IN(0), "Pointer 1");
+
+    /* Re-enable escape.  */
+    _swi(OSByte, _INR(0,2), osbyte_VAR_ESCAPE_STATE, 0, 0);
+#endif
 }
 
 void
@@ -326,12 +367,14 @@
 
     /* Start generating vsync callbacks if necesary */
     wdata->double_buffer = SDL_FALSE;
+#ifndef __RISCOS__
     if (SDL_GetHintBoolean(SDL_HINT_VIDEO_DOUBLE_BUFFER, SDL_FALSE)) {
         wdata->vsync_cond = SDL_CreateCond();
         wdata->vsync_cond_mutex = SDL_CreateMutex();
         wdata->double_buffer = SDL_TRUE;
         vc_dispmanx_vsync_callback(displaydata->dispman_display, RPI_vsync_callback, (void*)wdata);
     }
+#endif
 
     /* Setup driver data for this window */
     window->driverdata = wdata;
@@ -358,7 +401,9 @@
             SDL_CondWait(data->vsync_cond, data->vsync_cond_mutex);
             SDL_UnlockMutex(data->vsync_cond_mutex);
 
+#ifndef __RISCOS__
             vc_dispmanx_vsync_callback(displaydata->dispman_display, NULL, NULL);
+#endif
 
             SDL_DestroyCond(data->vsync_cond);
             SDL_DestroyMutex(data->vsync_cond_mutex);
@@ -368,6 +413,15 @@
         if (data->egl_surface != EGL_NO_SURFACE) {
             SDL_EGL_DestroySurface(_this, data->egl_surface);
         }
+#ifdef __RISCOS__
+        if (data->dispman_window.element > DISPMANX_NO_HANDLE) {
+           DISPMANX_UPDATE_HANDLE_T update;
+           update = vc_dispmanx_update_start( 10 );
+           SDL_assert( update );
+           vc_dispmanx_element_remove( update, data->dispman_window.element );
+           vc_dispmanx_update_submit_sync( update );
+        }
+#endif
 #endif
         SDL_free(data);
         window->driverdata = NULL;
