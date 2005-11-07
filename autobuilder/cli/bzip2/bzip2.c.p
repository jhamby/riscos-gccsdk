--- bzip2.c.orig	2005-11-06 23:35:24.000000000 -0800
+++ bzip2.c	2005-11-06 23:35:42.000000000 -0800
@@ -213,6 +213,11 @@
            ERROR_IF_MINUS_ONE ( retVal );               \
         } while ( 0 )
 #   endif
+
+#   ifdef __riscos__
+#     include <unixlib/local.h>
+#   endif
+
 #endif /* BZ_UNIX */
 
 
@@ -1230,6 +1235,10 @@
    FILE  *outStr;
    Int32 n, i;
    struct MY_STAT statBuf;
+#ifdef __riscos__
+   char *roname = __unixify_std(name, NULL, 0, 0);
+#define name roname
+#endif
 
    deleteOutputOnInterrupt = False;
 
@@ -1252,6 +1261,11 @@
          break;
    }
 
+#ifdef __riscos__
+#undef name
+   free (roname);
+#endif
+
    if ( srcMode != SM_I2O && containsDubiousChars ( inName ) ) {
       if (noisy)
       fprintf ( stderr, "%s: There are no files matching `%s'.\n",
@@ -1413,6 +1427,10 @@
    Bool  magicNumberOK;
    Bool  cantGuess;
    struct MY_STAT statBuf;
+#ifdef __riscos__
+   char *roname = __unixify_ext(name, NULL, 0, 0, "bz2");
+#define name roname
+#endif
 
    deleteOutputOnInterrupt = False;
 
@@ -1441,6 +1459,11 @@
    }
 
    zzz:
+#ifdef __riscos__
+#undef name
+   free (roname);
+#endif
+
    if ( srcMode != SM_I2O && containsDubiousChars ( inName ) ) {
       if (noisy)
       fprintf ( stderr, "%s: There are no files matching `%s'.\n",
