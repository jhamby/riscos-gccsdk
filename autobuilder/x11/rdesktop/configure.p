--- configure.old	2004-01-01 15:22:29.000000000 +0000
+++ configure	2004-01-01 15:22:30.000000000 +0000
@@ -262,8 +262,8 @@
 if [ -f /usr/include/sys/soundcard.h ]; then
     echo Sound support enabled: Open Sound System
     echo
-    echo "SOUNDOBJ    = rdpsnd.o rdpsnd_oss.o" >>Makeconf
-    cflags="$cflags -DWITH_RDPSND"
+    #echo "SOUNDOBJ    = rdpsnd.o rdpsnd_oss.o" >>Makeconf
+    #cflags="$cflags -DWITH_RDPSND"
 elif [ -f /usr/include/sys/audioio.h ]; then
     echo Sound support enabled: Sun
     echo
