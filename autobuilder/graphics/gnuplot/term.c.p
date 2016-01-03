--- term.c.orig	2002-02-01 19:46:04.000000000 +0100
+++ term.c	2012-04-19 09:59:32.000000000 +0200
@@ -45,7 +45,7 @@
   * terminal session lasts only until _either_ terminal
   * or output file changes. Before either is changed,
   * the terminal is shut down.
-  * 
+  *
   * Entry points : (see also term/README)
   *
   * term_set_output() : called when  set output  invoked
@@ -188,6 +188,9 @@
     else
 #endif
 	(void) fclose(gpoutfile);
+ #ifdef __riscos
+    RISCOS_set_type();
+#endif /* __riscos */
 
     gpoutfile = stdout;		/* Don't dup... */
     free(outstr);
@@ -574,18 +577,18 @@
 unsigned int x, y;
 int number;
 {
-    /* temporary(?) kludge to allow terminals with bad linetypes 
+    /* temporary(?) kludge to allow terminals with bad linetypes
        to make nice marks */
 
     (*term->linetype) (NICE_LINE);
     do_point(x, y, number);
 }
 
-/* 
+/*
  * general arrow routine
  *
  * I set the angle between the arrowhead and the line 15 degree.
- * The length of arrowhead varies depending on the line length 
+ * The length of arrowhead varies depending on the line length
  * within the the range [0.3*(the-tic-length), 2*(the-tic-length)].
  * No head is printed if the arrow length is zero.
  *
@@ -710,7 +713,7 @@
  */
 
 /* change angle of text.  0 is horizontal left to right.
-   * 1 is vertical bottom to top (90 deg rotate)  
+   * 1 is vertical bottom to top (90 deg rotate)
  */
 int null_text_angle(ang)
 int ang;
@@ -718,7 +721,7 @@
     return (ang == 0);
 }
 
-/* change justification of text.  
+/* change justification of text.
  * modes are LEFT (flush left), CENTRE (centred), RIGHT (flush right)
  */
 int null_justify_text(just)
@@ -952,9 +955,9 @@
 /*
  * Routine to detect what terminal is being used (or do anything else
  * that would be nice).  One anticipated (or allowed for) side effect
- * is that the global ``term'' may be set. 
+ * is that the global ``term'' may be set.
  * The environment variable GNUTERM is checked first; if that does
- * not exist, then the terminal hardware is checked, if possible, 
+ * not exist, then the terminal hardware is checked, if possible,
  * and finally, we can check $TERM for some kinds of terminals.
  * A default can be set with -DDEFAULTTERM=myterm in the Makefile
  * or #define DEFAULTTERM myterm in term.h
@@ -1133,7 +1136,7 @@
 void UP_redirect(caller)
 int caller;
 {
-    caller = caller;		/* to stop Turbo C complaining 
+    caller = caller;		/* to stop Turbo C complaining
 				   * about caller not being used */
 }
 
@@ -1208,7 +1211,7 @@
 	key_entry_height = (t->v_char);
 
     /* border linetype */
-    (*t->linewidth) (1.0); 
+    (*t->linewidth) (1.0);
     (*t->linetype) (-2);
     (*t->move) (0, 0);
     (*t->vector) (xmax_t - 1, 0);
@@ -1316,13 +1319,16 @@
 #if 0
 # if defined(MSDOS)||defined(g)||defined(MTOS)||defined(OS2)||defined(_Windows)||defined(DOS386)
 /* output for some terminal types must be binary to stop non Unix computers
-   changing \n to \r\n. 
-   If the output is not STDOUT, the following code reopens gpoutfile 
+   changing \n to \r\n.
+   If the output is not STDOUT, the following code reopens gpoutfile
    with binary mode. */
 void reopen_binary()
 {
     if (outstr) {
 	(void) fclose(gpoutfile);
+#  ifdef __riscos
+        RISCOS_set_type();
+#  endif /* __riscos */
 #  ifdef _Windows
 	if (!stricmp(outstr, "PRN")) {
 	    /* use temp file for windows */
@@ -1374,7 +1380,7 @@
 # include <dcdef.h>
 # include <stat.h>
 # include <fab.h>
-/* If you use WATCOM C or a very strict ANSI compiler, you may have to 
+/* If you use WATCOM C or a very strict ANSI compiler, you may have to
  * delete or comment out the following 3 lines: */
 # ifndef TT2$M_DECCRT3		/* VT300 not defined as of VAXC v2.4 */
 #  define TT2$M_DECCRT3 0X80000000
