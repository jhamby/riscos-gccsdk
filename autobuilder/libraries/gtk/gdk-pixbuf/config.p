--- gdk-pixbuf-config.in.old	2003-04-02 11:25:32.000000000 +0100
+++ gdk-pixbuf-config.in	2003-04-02 10:35:02.000000000 +0100
@@ -44,7 +44,7 @@
       ;;
     --libs)
       libdirs=-L@libdir@
-      echo $libdirs -lgdk_pixbuf `gtk-config --libs`
+      echo $libdirs -ljpeg -ltiff -lpng -lz -lgdk_pixbuf `gtk-config --libs`
       ;;
     *)
       echo "${usage}" 1>&2
