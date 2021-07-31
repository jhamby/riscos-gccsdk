--- syscfg.h.orig	2002-03-11 17:07:20.000000000 +0100
+++ syscfg.h	2016-01-01 20:35:58.722583848 +0100
@@ -60,9 +60,22 @@
  *           that is used to run external commands.
  * DIRSEP1:  ['/'] Primary character which separates path components.
  * DIRSEP2:  ['\0'] Secondary character which separates path components.
- *        
+ *
  */
 
+#if defined(__riscos)
+# define OS       "RISC OS "
+# define HELPFILE "<GnuPlot373$Dir>.gih.gnuplot"
+# define NAME     "GNUPLOT"
+# define PLOTRC   "<GnuPlot373$Dir>._gnuplot"
+# undef  SHELL
+# define DIRSEP1  '.'
+/* Something strange is happening... who def'd VMS? */
+# ifdef VMS
+# undef VMS
+# endif
+#endif /* RISC OS */
+
 #if defined(AMIGA_SC_6_1) || defined(AMIGA_AC_5) || defined(__amigaos__)
 # ifndef __amigaos__
 #  define OS "Amiga"
@@ -345,35 +358,35 @@
 # endif
 
 # ifndef HAVE_LIMITS_H
-#  define NO_LIMITS_H 
+#  define NO_LIMITS_H
 # endif
 
 # ifndef HAVE_LOCALE_H
-#  define NO_LOCALE_H 
+#  define NO_LOCALE_H
 # endif
 
 # ifndef HAVE_MATH_H
-#  define NO_MATH_H 
+#  define NO_MATH_H
 # endif
 
 # ifndef HAVE_STDLIB_H
-#  define NO_STDLIB_H 
+#  define NO_STDLIB_H
 # endif
 
 # ifndef HAVE_STRING_H
-#  define NO_STRING_H 
+#  define NO_STRING_H
 # endif
 
 # ifndef HAVE_TIME_H
-#  define NO_TIME_H 
+#  define NO_TIME_H
 # endif
 
 # ifndef HAVE_SYS_TIME_H
-#  define NO_SYS_TIME_H 
+#  define NO_SYS_TIME_H
 # endif
 
 # ifndef HAVE_SYS_TYPES_H
-#  define NO_SYS_TYPES_H 
+#  define NO_SYS_TYPES_H
 # endif
 
 # ifndef HAVE_ATEXIT
