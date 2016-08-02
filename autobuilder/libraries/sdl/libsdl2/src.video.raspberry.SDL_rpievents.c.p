--- src/video/raspberry/SDL_rpievents.c.orig	2016-01-02 19:56:31.000000000 +0000
+++ src/video/raspberry/SDL_rpievents.c	2016-07-23 19:44:22.075590169 +0100
@@ -33,12 +33,228 @@
 #include "../../core/linux/SDL_evdev.h"
 #endif
 
+#ifdef __riscos__
+#include <pthread.h>
+#include <swis.h>
+#include "oslib/wimp.h"
+#include "oslib/osbyte.h"
+
+static SDL_Scancode RISCOS_Keycodes[] = {
+    SDL_SCANCODE_UNKNOWN,  //0	Shift			
+    SDL_SCANCODE_UNKNOWN,  //1	Ctrl			
+    SDL_SCANCODE_UNKNOWN,  //2	Alt			
+    SDL_SCANCODE_LSHIFT,	/* 3 */
+    SDL_SCANCODE_LCTRL,		/* 4 */
+    SDL_SCANCODE_LALT,		/* 5 */
+    SDL_SCANCODE_RSHIFT,
+    SDL_SCANCODE_RCTRL,
+    SDL_SCANCODE_RALT,
+    SDL_SCANCODE_UNKNOWN,  //9	Left mouse
+    SDL_SCANCODE_UNKNOWN,  //10	Center mouse
+    SDL_SCANCODE_UNKNOWN,  //11	Right mouse
+    SDL_SCANCODE_UNKNOWN,	/* 12 */
+    SDL_SCANCODE_UNKNOWN,	/* 13 */
+    SDL_SCANCODE_UNKNOWN,	/* 14 */
+    SDL_SCANCODE_UNKNOWN,	/* 15 */
+    SDL_SCANCODE_Q,		/* 16 */
+    SDL_SCANCODE_3,		/* 17 */
+    SDL_SCANCODE_4,		/* 18 */
+    SDL_SCANCODE_5,		/* 19 */
+    SDL_SCANCODE_F4,		/* 20 */
+    SDL_SCANCODE_8,		/* 21 */
+    SDL_SCANCODE_F7,		/* 22 */
+    SDL_SCANCODE_MINUS,		/* 23 */
+    SDL_SCANCODE_UNKNOWN,	/* 24 */
+    SDL_SCANCODE_LEFT,		/* 25 */
+    SDL_SCANCODE_KP_6,		/* 26 */
+    SDL_SCANCODE_KP_7,		/* 27 */
+    SDL_SCANCODE_F11,		/* 28 */
+    SDL_SCANCODE_F12,		/* 29 */
+    SDL_SCANCODE_F10,		/* 30 */
+    SDL_SCANCODE_SCROLLLOCK,
+    SDL_SCANCODE_PRINTSCREEN,
+    SDL_SCANCODE_W,
+    SDL_SCANCODE_E,
+    SDL_SCANCODE_T,
+    SDL_SCANCODE_7,
+    SDL_SCANCODE_I,
+    SDL_SCANCODE_9,
+    SDL_SCANCODE_0,
+    SDL_SCANCODE_UNKNOWN,
+    SDL_SCANCODE_DOWN,
+    SDL_SCANCODE_KP_8,
+    SDL_SCANCODE_KP_9,
+    SDL_SCANCODE_PAUSE,
+    SDL_SCANCODE_UNKNOWN,  //45	~		
+    SDL_SCANCODE_UNKNOWN,  //46	£		
+    SDL_SCANCODE_BACKSPACE,
+    SDL_SCANCODE_1,
+    SDL_SCANCODE_2,
+    SDL_SCANCODE_D,
+    SDL_SCANCODE_R,
+    SDL_SCANCODE_6,
+    SDL_SCANCODE_U,
+    SDL_SCANCODE_O,
+    SDL_SCANCODE_P,
+    SDL_SCANCODE_LEFTBRACKET,
+    SDL_SCANCODE_UP,
+    SDL_SCANCODE_KP_PLUS,
+    SDL_SCANCODE_KP_MINUS,
+    SDL_SCANCODE_KP_ENTER,
+    SDL_SCANCODE_INSERT,
+    SDL_SCANCODE_HOME,
+    SDL_SCANCODE_PAGEUP,
+    SDL_SCANCODE_CAPSLOCK,
+    SDL_SCANCODE_A,
+    SDL_SCANCODE_X,
+    SDL_SCANCODE_F,
+    SDL_SCANCODE_Y,
+    SDL_SCANCODE_J,
+    SDL_SCANCODE_K,
+    SDL_SCANCODE_UNKNOWN,
+    SDL_SCANCODE_UNKNOWN,
+    SDL_SCANCODE_RETURN,
+    SDL_SCANCODE_KP_DIVIDE,
+    SDL_SCANCODE_UNKNOWN,
+    SDL_SCANCODE_KP_PERIOD,
+    SDL_SCANCODE_NUMLOCKCLEAR,
+    SDL_SCANCODE_PAGEDOWN,
+    SDL_SCANCODE_UNKNOWN,  //79	"
+    SDL_SCANCODE_UNKNOWN,
+    SDL_SCANCODE_S,
+    SDL_SCANCODE_C,
+    SDL_SCANCODE_G,
+    SDL_SCANCODE_H,
+    SDL_SCANCODE_N,
+    SDL_SCANCODE_L,
+    SDL_SCANCODE_SEMICOLON,
+    SDL_SCANCODE_RIGHTBRACKET,
+    SDL_SCANCODE_DELETE,
+    SDL_SCANCODE_KP_HASH,
+    SDL_SCANCODE_KP_MULTIPLY,
+    SDL_SCANCODE_UNKNOWN,
+    SDL_SCANCODE_EQUALS,
+    SDL_SCANCODE_UNKNOWN, //94	extra key
+    SDL_SCANCODE_UNKNOWN,
+    SDL_SCANCODE_TAB,
+    SDL_SCANCODE_Z,
+    SDL_SCANCODE_SPACE,
+    SDL_SCANCODE_V,
+    SDL_SCANCODE_B,
+    SDL_SCANCODE_M,
+    SDL_SCANCODE_COMMA,
+    SDL_SCANCODE_PERIOD,
+    SDL_SCANCODE_KP_DIVIDE,
+    SDL_SCANCODE_END,
+    SDL_SCANCODE_KP_0,
+    SDL_SCANCODE_KP_1,
+    SDL_SCANCODE_KP_3,
+    SDL_SCANCODE_UNKNOWN,
+    SDL_SCANCODE_UNKNOWN,
+    SDL_SCANCODE_UNKNOWN,
+    SDL_SCANCODE_ESCAPE,
+    SDL_SCANCODE_F1,
+    SDL_SCANCODE_F2,
+    SDL_SCANCODE_F3,
+    SDL_SCANCODE_F5,
+    SDL_SCANCODE_F6,
+    SDL_SCANCODE_F8,
+    SDL_SCANCODE_F9,
+    SDL_SCANCODE_UNKNOWN, /*120	\\ */
+    SDL_SCANCODE_RIGHT,
+    SDL_SCANCODE_KP_4,
+    SDL_SCANCODE_KP_5,
+    SDL_SCANCODE_KP_2,
+};
+
+static Uint8 translate_mouse_button(uint32_t b)
+{
+    if (b & wimp_CLICK_SELECT)
+	return SDL_BUTTON_LEFT;
+    else if (b & wimp_CLICK_MENU)
+	return SDL_BUTTON_MIDDLE;
+    else if (b & wimp_CLICK_ADJUST)
+	return SDL_BUTTON_RIGHT;
+
+    return 0;
+}
+
+static SDL_Scancode SDL_RISCOS_translate_keycode(int keycode)
+{
+    SDL_Scancode scancode = SDL_SCANCODE_UNKNOWN;
+
+    if (keycode < SDL_arraysize(RISCOS_Keycodes)) {
+        scancode = RISCOS_Keycodes[keycode];
+    }
+    if (scancode == SDL_SCANCODE_UNKNOWN) {
+        printf("The key you just pressed is not recognized by SDL. %d \n", keycode);
+    }
+    return scancode;
+}
+
+#endif
+
 void RPI_PumpEvents(_THIS)
 {
 #ifdef SDL_INPUT_LINUXEV
     SDL_EVDEV_Poll();
 #endif
-    
+#ifdef __riscos__
+    SDL_VideoData *driverdata = (SDL_VideoData *)_this->driverdata;
+    SDL_Mouse *mouse = SDL_GetMouse();
+    int x, y, buttons;
+    uint32_t screen_height = driverdata->screen_height;
+
+    _swix(OS_Mouse, _OUTR(0,2), &x, &y, &buttons);
+    x >>= driverdata->eigen_x;
+    y >>= driverdata->eigen_y;
+
+    if (mouse->x != x || mouse->y != screen_height - y)
+      SDL_SendMouseMotion(mouse->focus, mouse->mouseID, 0, x, screen_height - y);
+
+    if (driverdata->old_mouse_buttons != 0 && buttons == 0)
+      SDL_SendMouseButton(mouse->focus, mouse->mouseID, SDL_RELEASED, translate_mouse_button(driverdata->old_mouse_buttons));
+    else if (driverdata->old_mouse_buttons == 0 && buttons != 0)
+      SDL_SendMouseButton(mouse->focus, mouse->mouseID, SDL_PRESSED, translate_mouse_button(buttons));
+
+    driverdata->old_mouse_buttons = buttons;
+
+    // Check for key releases
+    int i;
+    for (i = 0; i < RISCOS_MAX_KEYS_PRESSED; i++) {
+	if (driverdata->key_pressed[i])
+	    if (_swi(OS_Byte, _INR(0,2)|_RETURN(1), osbyte_IN_KEY,
+						    driverdata->key_pressed[i] ^ 0xff,
+						    0xff) == 0) {
+		SDL_SendKeyboardKey(SDL_RELEASED, SDL_RISCOS_translate_keycode(driverdata->key_pressed[i]));
+		driverdata->key_pressed[i] = 0;
+	    }
+    }
+
+    // Check for key presses
+    uint8_t key = _swi(OS_Byte, _IN(0)|_RETURN(1), osbyte_SCAN_KEYBOARD_LIMITED);
+    if (key != 0xff) {
+	uint32_t already_pressed = 0;
+
+	// Do we already know of this key?
+	for (i = 0; i < RISCOS_MAX_KEYS_PRESSED; i++) {
+	    if (driverdata->key_pressed[i] == key) {
+		already_pressed = 1;
+		break;
+	    }
+	}
+	if (!already_pressed) {
+	    SDL_SendKeyboardKey(SDL_PRESSED, SDL_RISCOS_translate_keycode(key));
+	    // Record the press so we can detect release later.
+	    for (i = 0; i < RISCOS_MAX_KEYS_PRESSED; i++) {
+		if (driverdata->key_pressed[i] == 0) {
+		    driverdata->key_pressed[i] = key;
+		    break;
+		}
+	    }
+	}
+    }
+#endif
 }
 
 #endif /* SDL_VIDEO_DRIVER_RPI */
