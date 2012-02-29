--- configure.orig	2012-02-22 11:18:10.000000000 +0000
+++ configure	2012-02-22 11:21:17.000000000 +0000
@@ -1594,6 +1594,10 @@
   def_priority="#define CONFIG_PRIORITY 1"
 fi

+
+  _exesuf=",e1f"
+
+
 if wine ; then
   extra_cflags="-fno-pic -UWIN32 -U_WIN32 -U__WIN32 -U__WIN32__ -DWINE_NOWINSOCK -Dstricmp=strcasecmp $extra_cflags"
 fi
