--- audiofile.pc.in.orig	2013-02-11 17:23:26.000000000 +0000
+++ audiofile.pc.in	2020-11-29 23:44:25.477691816 +0000
@@ -5,7 +5,7 @@
 
 Name: audiofile
 Description: audiofile
-Requires:
+@ENABLE_FLAC_TRUE@Requires.private: flac >= 1.2.1
 Version: @VERSION@
 Libs: -L${libdir} -laudiofile
 Libs.private: -lm
