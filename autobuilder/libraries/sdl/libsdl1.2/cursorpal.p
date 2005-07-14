Index: src/video/riscos/SDL_riscosmouse.c
===================================================================
RCS file: /home/sdlweb/libsdl.org/cvs/SDL12/src/video/riscos/SDL_riscosmouse.c,v
retrieving revision 1.3
diff -u -r1.3 SDL_riscosmouse.c
--- src/video/riscos/SDL_riscosmouse.c	12 Feb 2005 18:01:30 -0000	1.3
+++ src/video/riscos/SDL_riscosmouse.c	13 Jul 2005 07:39:21 -0000
@@ -43,6 +43,20 @@
 
 extern int mouseInWindow;
 
+/* Area to save cursor palette colours changed by SDL.
+   Actual values will be read before we change to the SDL cursor */
+static Uint8 wimp_cursor_palette[2][5] = {
+  {1, 25, 255, 255, 255},
+  {3, 25, 255, 255, 255}
+};
+
+static int cursor_palette_saved = 0;
+
+void WIMP_SaveCursorPalette();
+void WIMP_RestoreWimpCursor();
+void WIMP_SetSDLCursorPalette();
+
+
 void RISCOS_FreeWMCursor(_THIS, WMcursor *cursor)
 {
     free(cursor->data);
@@ -130,27 +144,8 @@
 		current_cursor = NULL;
 	} else
 	{
-		if (current_cursor == NULL)
-		{
-			/* First time set up the mouse colours */
-			Uint8 block[5];
-
-			/* Set up colour 1 as white */
-			block[0] = 1;   /* Colour to change 1 - 3 */
-			block[1] = 25;  /* Set pointer colour */
-			block[2] = 255; /* red component*/
-			block[3] = 255; /* green component */
-			block[4] = 255; /* blue component*/
-			_kernel_osword(12, (int *)block);
-		
-			/* Set colour 3 to back */
-			block[0] = 3;   /* Colour to change 1 - 3 */
-			block[1] = 25;  /* Set pointer colour*/
-			block[2] = 0; /* red component*/
-			block[3] = 0; /* green component */
-			block[4] = 0; /* blue component*/
-			_kernel_osword(12, (int *)block);
-		}
+            WMcursor *old_cursor = current_cursor;
+            int full_screen = (this->screen == NULL) ? 0 : (this->screen->flags & SDL_FULLSCREEN);
 
 		if (cursor != current_cursor)
 		{
@@ -167,15 +162,27 @@
 			cursor_def[8] = ((int)(cursor->data) >> 16) & 0xFF; /* ... */
 			cursor_def[9] = ((int)(cursor->data) >> 24) & 0xFF; /* Most significant byte of pointer to data */
 
-			if (_kernel_osword(21, (int *)cursor_def) == 0)
+			if (_kernel_osword(21, (int *)cursor_def) != 0)
 			{
 				SDL_SetError("RISCOS couldn't create the cursor to show");
 				return(0);
 			}
 			current_cursor = cursor;
 		}
+        if (full_screen || mouseInWindow)
+        {
+            if (old_cursor == NULL)
+            {
+                /* First time or reshow in window, so save/setup palette */
+                if (full_screen == 0 && !cursor_palette_saved)
+                {
+                     WIMP_SaveCursorPalette();
+                }
+                WIMP_SetSDLCursorPalette();
+            }
 
-		if ((this->screen->flags & SDL_FULLSCREEN) || mouseInWindow) _kernel_osbyte(106, 2, 0);
+            _kernel_osbyte(106, 2, 0);
+        }
 	}
 	
 	return(1);
@@ -215,6 +222,7 @@
 {
 	WMcursor *cursor = current_cursor;
 	current_cursor = NULL;
+      cursor_palette_saved = 0;
 	RISCOS_ShowWMCursor(this, cursor);
 }
 
@@ -299,3 +307,69 @@
 
    return mode;
 }
+
+/* Save mouse cursor palette to be restore when we are no longer
+   defining a cursor */
+
+void WIMP_SaveCursorPalette()
+{
+    _kernel_swi_regs regs;
+    int colour;
+
+    for (colour = 0; colour < 2; colour++)
+    {
+      regs.r[0] = (int)wimp_cursor_palette[colour][0];
+      regs.r[1] = 25;
+      /* Read settings with OS_ReadPalette */
+      if (_kernel_swi(0x2f, &regs, &regs) == NULL)
+      {
+        wimp_cursor_palette[colour][2] = (unsigned char)((regs.r[2] >> 8) & 0xFF);
+        wimp_cursor_palette[colour][3] = (unsigned char)((regs.r[2] >> 16) & 0xFF);
+        wimp_cursor_palette[colour][4] = (unsigned char)((regs.r[2] >> 24) & 0xFF);
+      }
+    }
+
+    cursor_palette_saved = 1;
+}
+
+/* Restore the wimps cursor when we leave the SDL window */
+void WIMP_RestoreWimpCursor()
+{
+    int colour;
+
+    /* Reset to pointer shape 1 */
+    _kernel_osbyte(106, 1, 0);
+
+    /* Reset pointer colours */
+    if (cursor_palette_saved)
+    {
+      for (colour = 0; colour < 2; colour++)
+      {
+        _kernel_osword(12, (int *)wimp_cursor_palette[colour]);
+      }
+    }
+    cursor_palette_saved = 0;
+}
+
+/* Set palette used for SDL mouse cursors */
+void WIMP_SetSDLCursorPalette()
+{
+  /* First time set up the mouse colours */
+  Uint8 block[5];
+
+  /* Set up colour 1 as white */
+  block[0] = 1;   /* Colour to change 1 - 3 */
+  block[1] = 25;  /* Set pointer colour */
+  block[2] = 255; /* red component*/
+  block[3] = 255; /* green component */
+  block[4] = 255; /* blue component*/
+ _kernel_osword(12, (int *)block);
+		
+ /* Set colour 3 to back */
+ block[0] = 3;   /* Colour to change 1 - 3 */
+ block[1] = 25;  /* Set pointer colour*/
+ block[2] = 0; /* red component*/
+ block[3] = 0; /* green component */
+ block[4] = 0; /* blue component*/
+ _kernel_osword(12, (int *)block);
+}
Index: src/video/riscos/SDL_wimppoll.c
===================================================================
RCS file: /home/sdlweb/libsdl.org/cvs/SDL12/src/video/riscos/SDL_wimppoll.c,v
retrieving revision 1.4
diff -u -r1.4 SDL_wimppoll.c
--- src/video/riscos/SDL_wimppoll.c	12 Feb 2005 18:01:31 -0000	1.4
+++ src/video/riscos/SDL_wimppoll.c	13 Jul 2005 07:39:21 -0000
@@ -71,6 +71,7 @@
 
 /* Mouse cursor handling */
 extern void WIMP_ReshowCursor(_THIS);
+extern void WIMP_RestoreWimpCursor();
 
 int hasFocus = 0;
 int mouseInWindow = 0;
@@ -191,10 +192,7 @@
 				mouseInWindow = 0;
 				//TODO: Lose buttons / dragging
 				 /* Reset to default pointer */
-   				 regs.r[0] = 106;
-				 regs.r[1] = 1;
-				 regs.r[2] = 0;
-				 _kernel_swi(OS_Byte, &regs, &regs);
+				 WIMP_RestoreWimpCursor();
 				 SDL_PrivateAppActive(0, SDL_APPMOUSEFOCUS);
 			} else
 				sysEvent = 1;
