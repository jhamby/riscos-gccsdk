--- Imakefile.orig	2005-07-08 10:03:30.000000000 +0100
+++ Imakefile	2005-07-08 10:05:47.000000000 +0100
@@ -35,7 +35,7 @@
                       real.C vec2.C vec2list.C mat2.C \
                       color_mapper.C gif_image.C gifx_image.C
 
-           CXXFLAGS = $(USE_MIT_SHM) $(RANG_CHECK)
+           CXXFLAGS = $(RANG_CHECK) -I/home/riscos/env/include -O2
 
     LOCAL_LIBRARIES = $(XLIB) -lm
 
