--- terminal.c.org	2006-08-28 23:37:17.000000000 +0200
+++ terminal.c	2006-08-28 23:39:40.000000000 +0200
@@ -83,7 +83,7 @@
 static int tcap_initialized;
 
 #if !defined (__linux__)
-#  if defined (__EMX__) || defined (NEED_EXTERN_PC)
+#  if defined (__EMX__) || defined (__riscos__) || defined (NEED_EXTERN_PC)
 extern 
 #  endif /* __EMX__ || NEED_EXTERN_PC */
 char PC, *BC, *UP;
