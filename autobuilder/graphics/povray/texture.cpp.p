--- source/texture.cpp.orig	2004-08-03 00:11:38.000000000 +0100
+++ source/texture.cpp	2011-11-27 15:00:07.000000000 +0000
@@ -898,7 +898,7 @@
   if ((noise_generator>1) && (opts.Language_Version >= 350))
   {
       value = (2.0 * Noise(EPoint, TPat) - 0.5);
-      value = min(max(value,0.0),1.0);
+      value = min(max(value,static_cast<double>(0.0)),static_cast<double>(1.0));
 	} else {
       value = Noise(EPoint, TPat);
 	}
