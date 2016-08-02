--- src/video/raspberry/SDL_rpivideo.c.orig	2016-01-02 19:56:31.000000000 +0000
+++ src/video/raspberry/SDL_rpivideo.c	2016-07-23 19:47:27.159595477 +0100
@@ -38,6 +38,7 @@
 #include "SDL_events.h"
 #include "../../events/SDL_mouse_c.h"
 #include "../../events/SDL_keyboard_c.h"
+#include "SDL_assert.h"
 
 #ifdef SDL_INPUT_LINUXEV
 #include "../../core/linux/SDL_evdev.h"
@@ -49,6 +50,10 @@
 #include "SDL_rpiopengles.h"
 #include "SDL_rpimouse.h"
 
+#ifdef __riscos__
+#include "oslib/osbyte.h"
+#endif
+
 static int
 RPI_Available(void)
 {
@@ -187,6 +192,31 @@
     
     RPI_InitMouse(_this);
 
+#ifdef __riscos__
+    {
+        SDL_VideoData *driverdata = (SDL_VideoData *) _this->driverdata;
+	driverdata->screen_width = w;
+	driverdata->screen_height = h;
+	driverdata->old_mouse_buttons = 0;
+
+	os_VDU_VAR_LIST(3) var_list = { { os_VDUVAR_XEIG_FACTOR,
+					  os_VDUVAR_YEIG_FACTOR,
+					  os_VDUVAR_END_LIST } };
+	_swix(OS_ReadVduVariables, _INR(0,1), &var_list, &var_list);
+	driverdata->eigen_x = var_list.var[0];
+	driverdata->eigen_y = var_list.var[1];
+
+	int i;
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
 
@@ -196,6 +226,13 @@
 #ifdef SDL_INPUT_LINUXEV    
     SDL_EVDEV_Quit();
 #endif    
+#ifdef __riscos__
+    /* Turn the RISC OS pointer back on.  */
+    _swi(OS_CLI, _IN(0), "Pointer 1");
+
+    /* Re-enable escape.  */
+    _swi(OSByte, _INR(0,2), osbyte_VAR_ESCAPE_STATE, 0, 0);
+#endif
 }
 
 void
@@ -290,6 +327,15 @@
         if (data->egl_surface != EGL_NO_SURFACE) {
             SDL_EGL_DestroySurface(_this, data->egl_surface);
         }
+#ifdef __riscos__
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
