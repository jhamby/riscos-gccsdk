--- job.c.orig	2004-03-27 11:01:56.000000000 +0000
+++ job.c	2004-03-27 11:23:36.000000000 +0000
@@ -51,7 +51,11 @@
 #    include <descrip.h>
 char default_shell[] = "";
 #   else
+#    ifdef __riscos__
+char default_shell[] = "";
+#    else /* RISC OS */
 char default_shell[] = "/bin/sh";
+#    endif
 #   endif /* VMS */
 #  endif /* __MSDOS__ */
 int batch_mode_shell = 0;
@@ -1609,7 +1613,7 @@
 static int
 load_too_high ()
 {
-#if defined(__MSDOS__) || defined(VMS) || defined(_AMIGA)
+#if defined(__MSDOS__) || defined(VMS) || defined(_AMIGA) || defined(__riscos__)
   return 1;
 #else
   double load;
@@ -2479,12 +2483,17 @@
   char*  sh_chars;
   char** sh_cmds;
 #else  /* must be UNIX-ish */
+#ifdef __riscos__
+  static char sh_chars[] = "";
+  static char *sh_cmds[] = { 0 };
+#else  /* must be UNIX-ish */
   static char sh_chars[] = "#;\"*?[]&|<>(){}$`^~";
   static char *sh_cmds[] = { "cd", "eval", "exec", "exit", "login",
 			     "logout", "set", "umask", "wait", "while", "for",
 			     "case", "if", ":", ".", "break", "continue",
 			     "export", "read", "readonly", "shift", "times",
 			     "trap", "switch", 0 };
+#endif /* __riscos__ */
 #endif /* WINDOWS32 */
 #endif /* Amiga */
 #endif /* __MSDOS__ */
