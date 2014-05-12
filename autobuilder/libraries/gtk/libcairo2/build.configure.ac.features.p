--- build/configure.ac.features.orig	2012-02-02 00:48:14.000000000 +0000
+++ build/configure.ac.features	2013-07-18 20:46:44.354822208 +0100
@@ -389,6 +389,7 @@
 	echo "  OpenVG:        $use_vg"
 	echo "  DRM:           $use_drm"
 	echo "  Cogl:          $use_cogl"
+	echo "  RISC OS:       $use_riscos_sprite"
 	echo ""
 	echo "The following font backends:"
 	echo "  User:          yes (always builtin)"
@@ -396,6 +397,7 @@
 	echo "  Fontconfig:    $use_fc"
 	echo "  Win32:         $use_win32_font"
 	echo "  Quartz:        $use_quartz_font"
+	echo "  RISC OS:       $use_riscos_font"
 	echo ""
 	echo "The following functions:"
 	echo "  PNG functions:   $use_png"
