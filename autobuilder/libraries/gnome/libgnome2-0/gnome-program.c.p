--- libgnome/gnome-program.c.orig	2011-12-29 18:22:09.000000000 +0000
+++ libgnome/gnome-program.c	2011-12-29 18:22:56.000000000 +0000
@@ -68,6 +68,8 @@
 #define PATH_MAX 4096
 #endif
 
+#define MULTIARCH "@"
+
 struct _GnomeProgramPrivate {
     enum {
 	APP_UNINIT=0,
