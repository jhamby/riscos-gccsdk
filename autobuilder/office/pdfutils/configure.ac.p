--- configure.ac.orig	2014-05-18 15:58:43.440562168 +0100
+++ configure.ac	2014-05-18 15:59:01.820184176 +0100
@@ -824,7 +824,7 @@
 		-Wmissing-format-attribute -fno-exceptions -fno-check-new \
 		-fno-common $CXXFLAGS" ;;
 esac
-CXXFLAGS="-fPIC $CXXFLAGS";
+CXXFLAGS="$CXXFLAGS";
 
 case `$PKG_CONFIG --version` in
   0.?|0.1[0-7])
