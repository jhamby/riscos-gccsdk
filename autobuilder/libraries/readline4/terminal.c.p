--- terminal.c.old	2003-02-02 20:45:50.000000000 +0000
+++ terminal.c	2003-02-02 20:46:46.000000000 +0000
@@ -81,7 +81,7 @@
 static int tcap_initialized;
 
 #if !defined (__linux__)
-#  if defined (__EMX__) || defined (NEED_EXTERN_PC)
+#  if defined (__EMX__) || defined (__riscos__) || defined (NEED_EXTERN_PC)
 extern
 #  endif /* __EMX__ || NEED_EXTERN_PC */
 char PC, *BC, *UP;
