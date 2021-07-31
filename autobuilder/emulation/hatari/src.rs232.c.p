--- src/rs232.c.orig	2020-09-07 08:25:02.557590782 +0100
+++ src/rs232.c	2020-09-07 08:29:43.696129476 +0100
@@ -55,7 +55,7 @@
 #if HAVE_TERMIOS_H
 
 #if !HAVE_CFMAKERAW
-static inline void cfmakeraw(struct termios *termios_p)
+static inline void cfmakeraw2(struct termios *termios_p)
 {
 	termios_p->c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL|IXON);
 	termios_p->c_oflag &= ~OPOST;
@@ -119,7 +119,7 @@
 		/* Set "raw" mode: */
 		termmode.c_cc[VMIN] = 1;
 		termmode.c_cc[VTIME] = 0;
-		cfmakeraw(&termmode);
+		cfmakeraw2(&termmode);
 		if (tcsetattr(fd, TCSADRAIN, &termmode) != 0)
 			return false;
 	}
