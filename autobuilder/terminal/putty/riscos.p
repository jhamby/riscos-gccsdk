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
@@ -2625,7 +2628,7 @@
 	     * glyphs in the first 32 char positions, it is assumed
 	     * that those glyphs are the VT100 line-drawing
 	     * character set.
-	     * 
+	     *
 	     * Actually, we'll hack even harder by only checking
 	     * position 0x19 (vertical line, VT100 linedrawing
 	     * `x'). Then we can check it easily by seeing if the
@@ -3021,7 +3024,7 @@
      * Re-execing ourself is not an exact science under Unix. I do
      * the best I can by using /proc/self/exe if available and by
      * assuming argv[0] can be found on $PATH if not.
-     * 
+     *
      * Note that we also have to reconstruct the elements of the
      * original argv which gtk swallowed, since the user wants the
      * new session to appear on the same X display as the old one.
Index: unix/uxcons.c
===================================================================
--- unix/uxcons.c	(revision 5808)
+++ unix/uxcons.c	(working copy)
@@ -129,12 +129,13 @@
 
     {
 	struct termios oldmode, newmode;
+	int count;
 	tcgetattr(0, &oldmode);
 	newmode = oldmode;
 	newmode.c_lflag |= ECHO | ISIG | ICANON;
 	tcsetattr(0, TCSANOW, &newmode);
 	line[0] = '\0';
-	read(0, line, sizeof(line) - 1);
+	count = read(0, line, sizeof(line) - 1);
 	tcsetattr(0, TCSANOW, &oldmode);
     }
 
@@ -177,12 +178,13 @@
 
     {
 	struct termios oldmode, newmode;
+	int count;
 	tcgetattr(0, &oldmode);
 	newmode = oldmode;
 	newmode.c_lflag |= ECHO | ISIG | ICANON;
 	tcsetattr(0, TCSANOW, &newmode);
 	line[0] = '\0';
-	read(0, line, sizeof(line) - 1);
+	count = read(0, line, sizeof(line) - 1);
 	tcsetattr(0, TCSANOW, &oldmode);
     }
 
@@ -226,12 +228,13 @@
 
     {
 	struct termios oldmode, newmode;
+	int count;
 	tcgetattr(0, &oldmode);
 	newmode = oldmode;
 	newmode.c_lflag |= ECHO | ISIG | ICANON;
 	tcsetattr(0, TCSANOW, &newmode);
 	line[0] = '\0';
-	read(0, line, sizeof(line) - 1);
+	count = read(0, line, sizeof(line) - 1);
 	tcsetattr(0, TCSANOW, &oldmode);
     }
 
@@ -245,7 +248,7 @@
 
 /*
  * Warn about the obsolescent key file format.
- * 
+ *
  * Uniquely among these functions, this one does _not_ expect a
  * frontend handle. This means that if PuTTY is ported to a
  * platform which requires frontend handles, this function will be
Index: unix/uxpty.c
===================================================================
--- unix/uxpty.c	(revision 5808)
+++ unix/uxpty.c	(working copy)
@@ -257,7 +257,8 @@
 
 static void sigchld_handler(int signum)
 {
-    write(pty_signal_pipe[1], "x", 1);
+    int count;
+    count = write(pty_signal_pipe[1], "x", 1);
 }
 
 #ifndef OMIT_UTMP
@@ -286,6 +287,7 @@
     const char *p1, *p2;
     char master_name[20];
     struct group *gp;
+    int success;
 
     for (p1 = chars1; *p1; p1++)
 	for (p2 = chars2; *p2; p2++) {
@@ -323,7 +325,7 @@
 
     /* We need to chown/chmod the /dev/ttyXX device. */
     gp = getgrnam("tty");
-    chown(pty->name, getuid(), gp ? gp->gr_gid : -1);
+    success = chown(pty->name, getuid(), gp ? gp->gr_gid : -1);
     chmod(pty->name, 0600);
 #else
     pty->master_fd = open("/dev/ptmx", O_RDWR);
@@ -605,8 +607,9 @@
 	int ipid;
 	int status;
 	char c[1];
+	int count;
 
-	read(pty_signal_pipe[0], c, 1); /* ignore its value; it'll be `x' */
+	count = read(pty_signal_pipe[0], c, 1); /* ignore its value; it'll be `x' */
 
 	do {
 	    pid = waitpid(-1, &status, WNOHANG);
@@ -744,7 +747,7 @@
 	dup2(slavefd, 1);
 	dup2(slavefd, 2);
 	setsid();
-	ioctl(slavefd, TIOCSCTTY, 1);
+	ioctl(slavefd, TIOCSCTTY, (void *) 1);
 	pgrp = getpid();
 	tcsetpgrp(slavefd, pgrp);
 	setpgid(pgrp, pgrp);
Index: unix/uxplink.c
===================================================================
--- unix/uxplink.c	(revision 5808)
+++ unix/uxplink.c	(working copy)
@@ -461,7 +461,8 @@
 
 void sigwinch(int signum)
 {
-    write(signalpipe[1], "x", 1);
+    int count;
+    count = write(signalpipe[1], "x", 1);
 }
 
 /*
@@ -985,7 +986,8 @@
 	if (FD_ISSET(signalpipe[0], &rset)) {
 	    char c[1];
 	    struct winsize size;
-	    read(signalpipe[0], c, 1); /* ignore its value; it'll be `x' */
+	    int count;
+	    count = read(signalpipe[0], c, 1); /* ignore its value; it'll be `x' */
 	    if (ioctl(0, TIOCGWINSZ, (void *)&size) >= 0)
 		back->size(backhandle, size.ws_col, size.ws_row);
 	}
