--- base/configure.ac.orig	2009-01-25 06:59:45.000000000 +0000
+++ base/configure.ac	2012-09-25 15:38:01.000000000 +0100
@@ -803,6 +803,7 @@
 TIFF_DEVS="tiffs tiff12nc tiff24nc tiff32nc tiffcrle tifflzw tiffpack tiffgray tiffsep"
 PCX_DEVS="pcxmono pcxgray pcx16 pcx256 pcx24b pcxcmyk pcx2up"
 PBM_DEVS="pbm pbmraw pgm pgmraw pgnm pgnmraw pnm pnmraw ppm ppmraw pkm pkmraw pksm pksmraw pam"
+SPRITE_DEVS="spr spr1 spr8c spr8m sprm"
 PS_DEVS="psdf psdcmyk psdrgb pdfwrite pswrite ps2write epswrite psgray psmono psrgb bbox"
 # CAIRO_DEVS is defined conditionally above
 WTS_HALFTONING_DEVS="imdi simdi wtsimdi wtscmyk"
@@ -829,7 +830,7 @@
 		P_DEVS="$P_DEVS $CANON_DEVS $EPSON_DEVS $HP_DEVS $LEXMARK_DEVS $BROTHER_DEVS $APPLE_DEVS $IBM_DEVS $OKI_DEVS $JAPAN_DEVS $MISC_PDEVS $ETS_HALFTONING_DEVS $OPVP_DEVS"
 		;;
 	FILES)
-		F_DEVS="$F_DEVS $BMP_DEVS $FAX_DEVS $JPEG_DEVS $PNG_DEVS $TIFF_DEVS $PCX_DEVS $PBM_DEVS $PS_DEVS $CAIRO_DEVS $WTS_HALFTONING_DEVS $MISC_FDEVS"
+		F_DEVS="$F_DEVS $BMP_DEVS $FAX_DEVS $JPEG_DEVS $PNG_DEVS $TIFF_DEVS $PCX_DEVS $PBM_DEVS $SPRITE_DEVS $PS_DEVS $CAIRO_DEVS $WTS_HALFTONING_DEVS $MISC_FDEVS"
 		;;
 	APPLE)
 		# All Apple printers
@@ -875,6 +876,10 @@
 		# PBM file formats
 		F_DEVS="$F_DEVS $PBM_DEVS"
 		;;
+	SPRITE)
+		# SPRITE file formats
+		F_DEVS="$F_DEVS $SPRITE_DEVS"
+		;;
 	HP)
 		# All HP printers
 		P_DEVS="$P_DEVS $HP_DEVS"
