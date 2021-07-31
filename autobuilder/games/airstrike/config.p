--- src/config.h.org	2005-05-16 13:53:47.000000000 +0100
+++ src/config.h	2005-05-16 13:56:19.000000000 +0100
@@ -6,7 +6,7 @@
 /* absolute path to main config file (for now it's airstrikerc in the
    current dir)*/
 #ifndef ROOT_CONFIG_FILE
-#define ROOT_CONFIG_FILE "/etc/airstrikerc"
+#define ROOT_CONFIG_FILE "<AirStrike$Dir>/airstrikerc"
 #endif
 /* path to file in users $HOME */
 #ifndef USER_CONFIG_FILE
