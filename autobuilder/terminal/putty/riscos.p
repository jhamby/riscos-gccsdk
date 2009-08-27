Index: unix/gtkwin.c
===================================================================
--- unix/gtkwin.c	(revision 5808)
+++ unix/gtkwin.c	(working copy)
@@ -211,7 +211,7 @@
 /*
  * Translate a raw mouse button designation (LEFT, MIDDLE, RIGHT)
  * into a cooked one (SELECT, EXTEND, PASTE).
- * 
+ *
  * In Unix, this is not configurable; the X button arrangement is
  * rock-solid across all applications, everyone has a three-button
  * mouse or a means of faking it, and there is no need to switch
@@ -499,7 +499,7 @@
     /*
      * If Alt is being released after typing an Alt+numberpad
      * sequence, we should generate the code that was typed.
-     * 
+     *
      * Note that we only do this if more than one key was actually
      * pressed - I don't think Alt+NumPad4 should be ^D or that
      * Alt+NumPad3 should be ^C, for example. There's no serious
@@ -970,7 +970,7 @@
 	/*
 	 * Cursor keys. (This includes the numberpad cursor keys,
 	 * if we haven't already done them due to app keypad mode.)
-	 * 
+	 *
 	 * Here we also process un-numlocked un-appkeypadded KP5,
 	 * which sends ESC [ G.
 	 */
@@ -994,7 +994,7 @@
 		} else if (!inst->term->app_cursor_keys ^
 			   !(event->state & GDK_CONTROL_MASK)) {
 		    end = 1 + sprintf(output+1, "\033O%c", xkey);
-		} else {		    
+		} else {
 		    end = 1 + sprintf(output+1, "\033[%c", xkey);
 		}
 		use_ucsoutput = FALSE;
@@ -2259,6 +2259,9 @@
 	text[0] = (char)cursor_val + 1;
 	gdk_string_extents(cursor_font, text, &lb, &rb, &wid, &asc, &desc);
 	w = rb-lb; h = asc+desc; x = -lb; y = asc;
+	printf("make_mouse_ptr: gdk_string_extents returns w=%d h=%d x=%d y=%d rb=%d lb=%d asc=%d desc=%d",
+	  w,h,x,y,rb,lb,asc,desc);
+	fflush(stdout);
     } else {
 	w = h = 1;
 	x = y = 0;
@@ -3021,7 +3024,7 @@
      * Re-execing ourself is not an exact science under Unix. I do
      * the best I can by using /proc/self/exe if available and by
      * assuming argv[0] can be found on $PATH if not.
-     * 
+     *
      * Note that we also have to reconstruct the elements of the
      * original argv which gtk swallowed, since the user wants the
      * new session to appear on the same X display as the old one.
