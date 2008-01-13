--- zip.c.orig	2008-01-12 22:14:56.000000000 -0800
+++ zip.c	2008-01-12 22:40:36.000000000 -0800
@@ -476,7 +476,11 @@
 #endif /* ?AMIGA */
 #ifdef RISCOS
 ,"  -I   don't scan through Image files"
-#endif
+#else
+#ifdef FORRISCOS
+,"  -,   strip ,xxx from filename and convert to RISC OS filetype encoding"
+#endif /* FORRISCOS */
+#endif /* !RISCOS */
 #endif /* ?MACOS */
   };
 
@@ -557,6 +561,9 @@
 #if CRYPT && defined(PASSWD_FROM_STDIN)
     "PASSWD_FROM_STDIN",
 #endif /* CRYPT & PASSWD_FROM_STDIN */
+#ifdef FORRISCOS
+    "FORRISCOS",
+#endif
     NULL
   };
 
@@ -1246,6 +1253,12 @@
             case 'I':   /* Don't scan through Image files */
               scanimage = 0;
               break;
+#else
+#ifdef FORRISCOS
+            case ',':   /* Convert ,xxx to RISC OS extended filetype info */
+              decomma = 1;
+              break;
+#endif 
 #endif
 #ifdef MACOS
             case 'j':   /* Junk path / Store absolute path */
--- zip.h.org	2008-01-12 22:56:51.000000000 -0800
+++ zip.h	2008-01-12 23:00:05.000000000 -0800
@@ -256,6 +256,10 @@
 
 #ifdef RISCOS
 extern int scanimage;           /* Scan through image files */
+#else
+#ifdef FORRISCOS
+extern int decomma;             /* Convert ,xxx filename to RISC OS filetype info */
+#endif
 #endif
 
 #define BEST -1                 /* Use best method (deflation or store) */
