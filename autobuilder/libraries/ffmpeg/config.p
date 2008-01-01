Index: configure
===================================================================
RCS file: /cvsroot/ffmpeg/ffmpeg/configure,v
retrieving revision 1.190
diff -u -r1.190 configure
--- configure	11 Jul 2005 00:15:36 -0000	1.190
+++ configure	20 Jul 2005 22:20:14 -0000
@@ -360,7 +360,15 @@
 ffserver="no"
 vhook="no"
 os2="yes"
-
+;;
+RISCOS)
+v4l="no"
+dv1394="no"
+LDFLAGS=""
+FFSLDFLAGS=""
+EXESUF="$EXEEXT"
+ranlib="echo ignoring ranlib"
+strip="echo ignoring strip"
 ;;
 *) ;;
 esac
