--- configure.ac.orig	2009-08-28 12:12:38.000000000 -0700
+++ configure.ac	2009-08-28 12:15:40.000000000 -0700
@@ -1257,7 +1257,9 @@
   VLC_ADD_PLUGIN([marq])
   VLC_ADD_PLUGIN([podcast])
   VLC_ADD_PLUGIN([shout])
+if test "${SYS}" != "riscos"; then
   VLC_ADD_PLUGIN([sap])
+fi
   VLC_ADD_PLUGIN([fake])
   VLC_ADD_PLUGIN([folder])
   VLC_ADD_PLUGIN([rss])
