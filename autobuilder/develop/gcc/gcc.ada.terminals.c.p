--- gcc/ada/terminals.c.orig	2021-04-08 04:56:27.985739666 -0700
+++ gcc/ada/terminals.c	2021-08-07 21:45:09.924343696 -0700
@@ -34,7 +34,8 @@
 /* First all usupported platforms. Add stubs for exported routines. */
 
 #if defined (VMS) || defined (__vxworks) || defined (__Lynx__) \
-  || defined (__ANDROID__) || defined (__PikeOS__) || defined(__DJGPP__)
+  || defined (__ANDROID__) || defined (__PikeOS__) || defined(__DJGPP__) \
+  || defined (__riscos__)
 
 void *
 __gnat_new_tty (void)
@@ -1244,7 +1245,7 @@
       return -1;
     }
 
-#if !defined(__rtems__)
+#if !defined(__rtems__) && !defined(__riscos__)
   /* grant access to the slave side */
   grantpt (master_fd);
   /* unlock the terminal */
