--- sys/share/tclib.c.old	2003-02-01 15:23:28.000000000 +0000
+++ sys/share/tclib.c	2003-02-01 15:24:22.000000000 +0000
@@ -18,7 +18,7 @@
 /* exported variables, as per man page */
 char  PC;
 char *BC, *UP;
-short ospeed;
+extern short ospeed;
 
 /* exported routines */
 int   FDECL(tgetent,  (char *,const char *));
--- sys/share/unixtty.c.old	2003-02-01 15:32:51.000000000 +0000
+++ sys/share/unixtty.c	2003-02-01 15:33:44.000000000 +0000
@@ -121,7 +121,7 @@
 
 #endif	/* V7 */
 
-#if defined(TTY_GRAPHICS) && ((!defined(SYSV) && !defined(HPUX)) || defined(UNIXPC) || defined(SVR4))
+#if defined(TTY_GRAPHICS) && ((!defined(SYSV) && !defined(HPUX)) || defined(UNIXPC) || defined(SVR4) || defined(__riscos__))
 # ifndef LINT
 extern			/* it is defined in libtermlib (libtermcap) */
 # endif
