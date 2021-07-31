--- plot.c.orig	2002-03-11 17:10:44.000000000 +0100
+++ plot.c	2012-05-19 18:25:12.000000000 +0200
@@ -1,5 +1,5 @@
 #ifndef lint
-static char *RCSid = "$Id: plot.c,v 1.18.2.5 2002/03/11 16:10:44 lhecking Exp $";
+static char *RCSid = "$Id: plot.c,v 1.18.2.2 1999/09/23 13:26:36 lhecking Exp $";
 #endif
 
 /* GNUPLOT - plot.c */
@@ -34,21 +34,46 @@
  * to the extent permitted by applicable law.
 ]*/
 
-#ifdef HAVE_CONFIG_H
-# include "config.h"
+#ifdef __riscos
+#ifdef BB_UNIXLIBBUG
+#include <sys/utsname.h>
+#if BB_UNIXLIBBUG == 1
+#define __UNIXLIB_INTERNALS
+#ifdef __GNUC__
+/*#include <sys/unix.h>*/
+#else
+#include <sys/unix.h>
 #endif
+#undef __UNIXLIB_INTERNALS
+#elif BB_UNIXLIBBUG == 2
+  /* Version of unixlib with sigsetjmp/siglongjmp (see below). */
+#else  /* BB_UNIXLIBBUG != 1 */
+   /* Using a version of unixlib that has things fixed. */
+#endif
+#else /* ! BB_UNIXLIBBUG */
+#error Please define macro BB_UNIXLIBBUG, see README.
+#endif /* ! BB_UNIXLIBBUG */
+#endif /* __riscos */
 
 #include <signal.h>
-#include <setjmp.h>
 
 #include "plot.h"
 #include "fit.h"
 #include "setshow.h"
 #include "fnproto.h"
+#include <setjmp.h>
 
-#ifdef HAVE_SYS_UTSNAME_H
-# include <sys/utsname.h>
-#endif
+#if defined(__riscos) && defined(BB_UNIXLIBBUG)
+#if BB_UNIXLIBBUG == 2
+  /* Version of unixlib with sigsetjmp/siglongjmp. */
+#ifdef setjmp
+#undef setjmp
+#endif
+#define setjmp(env) sigsetjmp(env,1)
+#define longjmp(env,val) siglongjmp(env,val)
+#define jmp_buf sigjmp_buf
+#endif /* BB_UNIXLIBBUG == 2*/
+#endif /* __riscos && BB_UNIXLIBBUG */
 
 #if defined(MSDOS) || defined(DOS386) || defined(__EMX__)
 # include <io.h>
@@ -81,6 +106,7 @@
 # endif
 #endif /* _Windows */
 
+
 extern FILE *gpoutfile;
 
 TBOOLEAN interactive = TRUE;	/* FALSE if stdin not a terminal */
@@ -114,9 +140,9 @@
 
 /* a longjmp buffer to get back to the command line */
 #ifdef _Windows
-static JMP_BUF far command_line_env;
+static jmp_buf far command_line_env;
 #else
-static JMP_BUF command_line_env;
+static jmp_buf command_line_env;
 #endif
 
 static void load_rcfile __PROTO((void));
@@ -249,10 +275,39 @@
 RETSIGTYPE inter(anint)
 int anint;
 {
+#ifdef __riscos
+   /*printf("RB: SIGINT raised\n");*/ /* bb */
+#ifdef BB_UNIXLIBBUG
+#if    BB_UNIXLIBBUG == 1
+/* There is a bug in unixlib 3.7b, and 3.8 at least up to snapshot
+   20010520.  When longjmp()ing out of a signal handler the signal
+   remains blocked. So explicitely unblock it if this macro is
+   defined. */
+   {
+      sigset_t sset = sigmask(SIGINT);
+      __u->sigstate.currently_handling = 0;
+      if (sigprocmask(SIG_UNBLOCK, &sset, (sigset_t*)NULL))
+      {
+        /* Serious problem if we could not unblock signal before
+           the longjmp(). */
+        fprintf(stderr,
+                "Serious unixlib problem: could not unblock signal.\n");
+        exit(1);
+      }
+   }
+#else  /* BB_UNIXLIBBUG != 1 */
+   /* Using a version of unixlib that has things fixed. */
+#endif /* BB_UNIXLIBBUG != 1 */
+#else /* ! BB_UNIXLIBBUG */
+#error Please define macro BB_UNIXLIBBUG, see README.
+#endif /* ! BB_UNIXLIBBUG */
+#endif /* __riscos */
+
 #ifdef OS2
     (void) signal(anint, SIG_ACK);
 #else
     (void) signal(SIGINT, (sigfunc) inter);
+    /*printf("RO: resetting SIGINT\n"); */ /* bb */
 #endif
 
 #ifndef DOSX286
@@ -263,7 +318,7 @@
 #else
     term_reset();
     (void) putc('\n', stderr);
-    LONGJMP(command_line_env, TRUE);	/* return to prompt */
+    longjmp(command_line_env, TRUE);	/* return to prompt */
 #endif
 }
 
@@ -319,7 +374,7 @@
 /* a wrapper for longjmp so we can keep everything local */
 void bail_to_command_line()
 {
-    LONGJMP(command_line_env, TRUE);
+    longjmp(command_line_env, TRUE);
 }
 
 #if defined(_Windows) || defined(_Macintosh)
@@ -455,26 +510,25 @@
 	interactive = noinputfiles = FALSE;
     else
 	noinputfiles = TRUE;
-
     /* Need this before show_version is called for the first time */
 
 #ifdef HAVE_SYS_UTSNAME_H
     {
-	struct utsname uts;
+        struct utsname uts;
 
-	/* something is fundamentally wrong if this fails ... */
-	if (uname(&uts) > -1) {
+        /* something is fundamentally wrong if this fails ... */
+        if (uname(&uts) > -1) {
 # ifdef _AIX
-	    strcpy(os_name, uts.sysname);
-	    sprintf(os_name, "%s.%s", uts.version, uts.release);
+            strcpy(os_name, uts.sysname);
+            sprintf(os_name, "%s.%s", uts.version, uts.release);
 # elif defined(SCO)
-	    strcpy(os_name, "SCO");
-	    strcpy(os_rel, uts.release);
+            strcpy(os_name, "SCO");
+            strcpy(os_rel, uts.release);
 # else
-	    strcpy(os_name, uts.sysname);
-	    strcpy(os_rel, uts.release);
+            strcpy(os_name, uts.sysname);
+            strcpy(os_rel, uts.release);
 # endif
-	}
+        }
     }
 #else /* ! HAVE_SYS_UTSNAME_H */
 
@@ -738,10 +792,10 @@
  */
 {
     int rc;
-    static JMP_BUF keepenv;
+    static jmp_buf keepenv;
     int cmdlen;
 
-    memcpy(keepenv, command_line_env, sizeof(JMP_BUF));
+    memcpy(keepenv, command_line_env, sizeof(jmp_buf));
     if (!SETJMP(command_line_env, 1)) {
 	/* set variable input_line.
 	 * Watch out for line length of NOT_ZERO_TERMINATED strings ! */
@@ -758,7 +812,7 @@
 	*pusErr = RXSUBCOM_ERROR;
 	RexxSetHalt(getpid(), 1);
     }
-    memcpy(command_line_env, keepenv, sizeof(JMP_BUF));
+    memcpy(command_line_env, keepenv, sizeof(jmp_buf));
     return 0;
 }
 #endif
