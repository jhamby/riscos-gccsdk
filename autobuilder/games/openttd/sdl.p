Index: src/video/sdl_v.cpp
===================================================================
--- src/video/sdl_v.cpp	(revision 17960)
+++ src/video/sdl_v.cpp	(working copy)
@@ -25,6 +25,13 @@
 #include "sdl_v.h"
 #include <SDL.h>
 
+#if defined __riscos__
+#include "kernel.h"
+// This is a fudge because SDL_WarpMouse doesn't work in full screen
+// mode under RISC OS, for some reason. More in static int PollEvent(void)
+static int x_down, y_down;
+#endif
+
 static FVideoDriver_SDL iFVideoDriver_SDL;
 
 static SDL_Surface *_sdl_screen;
@@ -327,7 +334,6 @@
 
 	for (map = _vk_mapping; map != endof(_vk_mapping); ++map) {
 		if ((uint)(sym->sym - map->vk_from) <= map->vk_count) {
-			key = sym->sym - map->vk_from + map->map_to;
 			break;
 		}
 	}
@@ -346,6 +352,9 @@
 	if (sym->scancode == 49) key = WKC_BACKSPACE;
 #elif defined(__sgi__)
 	if (sym->scancode == 22) key = WKC_BACKQUOTE;
+#elif defined(__riscos__)
+	if (sym->scancode == 45) key |= WKC_BACKQUOTE; // RISC OS console
+			key = sym->sym - map->vk_from + map->map_to;
 #else
 	if (sym->scancode == 49) key = WKC_BACKQUOTE;
 #endif
@@ -370,11 +379,23 @@
 			if (_cursor.fix_at) {
 				int dx = ev.motion.x - _cursor.pos.x;
 				int dy = ev.motion.y - _cursor.pos.y;
+#if defined __riscos__
+			if (_fullscreen) {
+				dx = ev.motion.xrel;
+				dy = ev.motion.yrel;
+			}
+#endif
 				if (dx != 0 || dy != 0) {
 					_cursor.delta.x += dx;
 					_cursor.delta.y += dy;
+#if defined __riscos__
+				if (!_fullscreen) {
 					SDL_CALL SDL_WarpMouse(_cursor.pos.x, _cursor.pos.y);
 				}
+#else
+					SDL_CALL SDL_WarpMouse(_cursor.pos.x, _cursor.pos.y);
+#endif
+				}
 			} else {
 				_cursor.delta.x = ev.motion.x - _cursor.pos.x;
 				_cursor.delta.y = ev.motion.y - _cursor.pos.y;
@@ -431,6 +452,12 @@
 				UndrawMouseCursor(); // mouse left the window, undraw cursor
 				_cursor.in_window = false;
 			}
+#if defined __riscos__
+		if (_fullscreen) {
+			x_down = ev.button.x;
+			y_down = ev.button.y;
+		}
+#endif
 			break;
 
 		case SDL_QUIT:
@@ -444,6 +471,39 @@
 			} else {
 				HandleKeypress(ConvertSdlKeyIntoMy(&ev.key.keysym));
 			}
+#if defined __riscos__
+                if (_fullscreen && _cursor.fix_at) {
+			_kernel_swi_regs sRegs;
+			char acData[9];
+			int x_move;
+			int y_move;
+			_kernel_swi((0x1c), & sRegs, & sRegs);
+			x_move = (sRegs.r[0]) - 2 * (ev.button.x - x_down);
+			y_move = (sRegs.r[1]) + 2 * (ev.button.y - y_down);
+
+			acData[0] = 1;
+			acData[1] = ((-2*_cur_resolution.width) & 0xff);
+			acData[2] = ((-2*_cur_resolution.width) & 0xff00) >> 8;
+			acData[3] = ((-2*_cur_resolution.height) & 0xff);
+			acData[4] = ((-2*_cur_resolution.height) & 0xff00) >> 8;
+			acData[5] = ((4*_cur_resolution.width) & 0xff);
+			acData[6] = ((4*_cur_resolution.width) & 0xff00) >> 8;
+			acData[7] = ((4*_cur_resolution.height) & 0xff);
+			acData[8] = ((4*_cur_resolution.height) & 0xff00) >> 8;
+			sRegs.r[0] = 21;
+			sRegs.r[1] = (unsigned int)acData;
+			_kernel_swi((0x07), & sRegs, & sRegs);
+
+			acData[0] = 3;
+			acData[1] = (x_move & 0xff);
+			acData[2] = (x_move & 0xff00) >> 8;
+			acData[3] = (y_move & 0xff);
+			acData[4] = (y_move & 0xff00) >> 8;
+			sRegs.r[0] = 21;
+			sRegs.r[1] = (unsigned int)acData;
+			_kernel_swi((0x07), & sRegs, & sRegs);
+		}
+#endif
 			break;
 
 		case SDL_VIDEORESIZE: {
